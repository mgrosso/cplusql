#ident "file_id $Id: ThreadedByteSource.cpp,v 1.2 2004/08/11 21:42:44 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ThreadedByteSource.cpp
// Author:    mgrosso 
// Created:   Sat Jul  3 14:03:51 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: ThreadedByteSource.cpp,v 1.2 2004/08/11 21:42:44 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ThreadedByteSource.h"
#include "Exclusion.h"
#include "Event.h"
#include "ByteBuf.h"
#include "AppLog.h"
#include <list>

using namespace std;

typedef list< ByteBuf * >                   buffer_list_t ;
class                                       TBSReader ;

class ThreadedByteSourcePriv  
{
    ThreadedByteSourcePriv(
        ByteSourcePtr &bsp, size_t buffer_count, size_t buffer_size);
    private:
    friend class ThreadedByteSource ;
    friend class TBSReader ;
    ByteSourcePtr                       source_ ;
    Mutex                               m_ ;
    Condition                           c_ ;
    bool                                done_ ;
    buffer_list_t                       bufs_ ;
    EventQueuePtr                       q_ ;
    ByteBuf                            *current_ ;
};

class TBSReader : public EventHandler 
{
    public:
    ThreadedByteSourcePriv      *d_ ;
    virtual void  handleEvent( void *event );
    TBSReader( ThreadedByteSourcePriv *d);
};

TBSReader::TBSReader( ThreadedByteSourcePriv *d)
:d_(d)
{
    ;//noop
}

void
TBSReader::handleEvent( void *event)
{
    ByteBuf * b = static_cast< ByteBuf * >( event);
    b->rewind();
    size_t bytes_read =0;
    {
        MutexGuard mg(d_->m_);
        if( d_->done_ )
        {
            delete b;
            return;
        }
    }
    bool ret = d_->source_->next( b->buf(), bytes_read, b->bytesUsed());
    MutexGuard mg(d_->m_);
    if( ! ret )
    {
        d_->done_ = true ;
        delete b;
    }
    else
    {
        b->shrink( bytes_read );
        d_->bufs_.push_back( b );
    }
    d_->c_.signal();
}

ThreadedByteSourcePriv::ThreadedByteSourcePriv(
        ByteSourcePtr &bsp, size_t buffer_count, size_t buffer_size)
:source_(bsp),done_(false),current_(NULL)
{
    pEventFactory pef=NULL;
    pEventHandler peh=new TBSReader( this );
    if( buffer_count < 2 )
    {
        buffer_count = 2 ;
    }
    q_ = new EventQueue( pef, peh, 0, 1, buffer_count + 1 );
    for( size_t i =0; i< buffer_count; ++i )
    {
        q_->push(new ByteBuf( buffer_size, buffer_size ));
    }
}

ThreadedByteSource::ThreadedByteSource(
    ByteSourcePtr &bsp, 
    size_t num_buffers, size_t buffer_size )
{
    d_=new ThreadedByteSourcePriv( bsp, num_buffers, buffer_size );
}

ThreadedByteSource::~ThreadedByteSource()
{
    d_->q_->halt();
    while(! d_->bufs_.empty())
    {
        delete d_->bufs_.front();
        d_->bufs_.pop_front();
    }
    delete d_;
}

bool
ThreadedByteSource::next( char * &buf, size_t &len )
{
    MutexGuard mg(d_->m_);
    while( 1 )
    {
        if( d_->current_ )
        {
            if( d_->done_ )
            {
                delete d_->current_;
            }
            else
            {
                d_->q_->push( d_->current_ );
            }
            d_->current_=NULL;
        }
        if( ! d_->bufs_.empty())
        {
            d_->current_= d_->bufs_.front();
            d_->bufs_.pop_front();
            buf = d_->current_->buf();
            len = d_->current_->currentLength();
            return true;
        }
        else
        {
            if( d_->done_ )
            {
                return false;
            }
            d_->c_.wait( mg );
        }
    }
    //not reached.
    return false;
}

