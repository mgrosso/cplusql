#ident "file_id $Id: DestThreadedBuffer.cpp,v 1.8 2005/01/05 01:51:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestThreadedBuffer.cpp
// Author:    mgrosso 
// Created:   Tue Jun 22 09:22:05 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: DestThreadedBuffer.cpp,v 1.8 2005/01/05 01:51:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string>
#include <list>

#include "BaseException.h"
#include "ptr.h"

#include "frame_keys.h"

#include "DestThreadedBuffer.h"
#include "Exclusion.h"
#include "Event.h"
#include "ByteBuf.h"

#include "AppLog.h"

using namespace std ;

class BufWriter : public EventHandler 
{
    public:
    BufWriter(DestThreadedBufferPriv *d );
    virtual ~BufWriter();
    void handleEvent( void *e );
    DestThreadedBufferPriv *d_;
};

class DestThreadedBufferPriv 
{
    private:
    friend class DestThreadedBuffer ;
    friend class BufWriter ;

    DestThreadedBufferPriv(
        pDest &dest, char record_delim, 
        size_t buffer_count, size_t buffer_size,
        bool use_threads_ );

    pDest               dest_ ;
    char                record_delim_ ;
    size_t              buffer_count_ ;
    size_t              buffer_size_ ;
    bool                use_threads_ ;

    bool                puterr_ ;
    string              errbuf_;

    Mutex               lock_;
    Condition           flush_;

    ByteBuf *           current_ ;
    list< ByteBuf * >   available_ ;
    EventQueuePtr       writeq_ ;

    //helper functions
    static const char * chopat( const char *s, char c ,
        size_t end, size_t buffer_left );
    void                bufswap();
    void                empty_wait( MutexGuard &mg );
};

void
BufWriter::handleEvent( void *e )
{
    ByteBuf *buf = reinterpret_cast< ByteBuf *>(e);
    try{
        //APPLOG_DBG("buf=%lx len=%lu", reinterpret_cast< unsigned long >(e), buf->currentLength());
        d_->dest_->put(buf->get(), buf->currentLength()); 
    }catch( std::exception &e )
    {
        MutexGuard mg(d_->lock_);
        d_->errbuf_ = e.what();
        d_->puterr_ = true ;
        d_->flush_.signal();
        return;
    }
    MutexGuard mg(d_->lock_);
    d_->available_.push_front(buf);
    d_->flush_.signal();
};

const char * 
DestThreadedBufferPriv::chopat( const char *s, char c,
        size_t len, size_t buffer_left )
{
    if(  len < buffer_left )
    {
        return NULL;
    }
    if( ! c )
    {
        return NULL;
    }
    for( const char *e =s + buffer_left - 1; e >= s ; --e )
    {
        if( *e == c )
        {
            return e;
        }
    }
    return NULL;
}

BufWriter::BufWriter( DestThreadedBufferPriv *d )
:d_(d)
{
    ;//noop
}

BufWriter::~BufWriter()
{
    ;//noop
}

DestThreadedBufferPriv::DestThreadedBufferPriv(
    pDest &dest, char record_delim, 
    size_t buffer_count, size_t buffer_size,
    bool use_threads
)
:dest_(dest),
    record_delim_(record_delim),
    buffer_count_(buffer_count),
    buffer_size_(buffer_size),
    use_threads_(use_threads),
    puterr_(false),
    errbuf_("")
{
    current_ = new ByteBuf( buffer_size, buffer_size );
    if( buffer_count_ < 3 )
    {
        buffer_count_ = 3 ;
    }
    for( size_t i =1; i< buffer_count_; ++i )
    {
        available_.push_front(new ByteBuf( buffer_size, buffer_size ));
    }
    pEventHandler peh; 
    pEventHandler phook;
    size_t numHandlers ;
    if( use_threads_ )
    {
        peh =new BufWriter( this );
        numHandlers = 1 ;
    }
    else
    {
        phook =new BufWriter( this );
        numHandlers = 0 ;
    }
    pEventFactory pef ;
    writeq_ = new EventQueue( pef, peh, phook, 
        0, numHandlers, buffer_count + 1 );
}

DestThreadedBuffer::DestThreadedBuffer(
        pDest &dest, char record_delim, 
        size_t buffer_count, size_t buffer_size,
        bool use_threads )
{
    d_=new DestThreadedBufferPriv (
        dest, record_delim, buffer_count, buffer_size, use_threads );
    //APPLOG_DBG("%x, ", static_cast<int>(d_->record_delim_), d_->buffer_count_, d_->buffer_size_);
}

DestThreadedBuffer::~DestThreadedBuffer()
{
    if( d_->current_ )
    {
        delete d_->current_ ;
    }
    while( ! d_->available_.empty())
    {
        //APPLOG_DBG("front, %i", d_->available_.size());
        delete d_->available_.front();
        d_->available_.pop_front();
    }
    //APPLOG_DBG("priv");
    delete d_;
}

void
DestThreadedBuffer::put( const char *s )
{
    put( s, strlen(s));
}

void
DestThreadedBuffer::put( const char *s, int slen )
{
    //APPLOG_DBG("delim=%i", d_->record_delim_ );
    size_t remainder = 
        d_->current_->bytesUsed() - d_->current_->currentLength();
    const char *e = DestThreadedBufferPriv::chopat( 
        s, d_->record_delim_, slen, remainder);
    if( !e )
    {
        //APPLOG_DBG("slen=%i, remainder=%lu, buffer_size=%lu, curr len=%lu",
        //    slen, remainder, d_->buffer_size_, d_->current_->currentLength());
        if( d_->current_->hasRoom( slen ))
        {
            d_->current_->append( s, slen );
        }
        else
        {
            d_->bufswap();
            d_->current_->append( s, slen );
        }
        //APPLOG_DBG("slen=%i, remainder=%lu, buffer_size=%lu, curr len=%lu",
        //    slen, remainder, d_->buffer_size_, d_->current_->currentLength());
        return;
    }
    //APPLOG_DBG("slen=%i, remainder=%lu, buffer_size=%lu, curr len=%lu, e=%lu",
    //    slen, remainder, d_->buffer_size_, d_->current_->currentLength(), e - d_->current_->get());

    d_->bufswap();

    //APPLOG_DBG("slen=%i, remainder=%lu, buffer_size=%lu, curr len=%lu",
    //    slen, remainder, d_->buffer_size_, d_->current_->currentLength());

    size_t zslen = slen ;
    //APPLOG_DBG("slen=%i, zslen=%lu", slen, zslen );
    d_->current_->append( s, zslen );

    //APPLOG_DBG("slen=%i, remainder=%lu, buffer_size=%lu, curr len=%lu",
    //    slen, remainder, d_->buffer_size_, d_->current_->currentLength());
}

//flushes current_, and leaves a new current in place, which might have some
//data left over from the last one.
void
DestThreadedBufferPriv::bufswap()
{
    ByteBuf * b = current_ ;
    current_ = NULL;
    ByteBufPtr prev = b ;
    {
        MutexGuard mg(lock_);
        if( puterr_ )
        {
            PANICV( errbuf_.c_str());
        }
        if( available_.empty())
        {
            flush_.wait( mg );
        }
        if( puterr_ )
        {
            PANICV( errbuf_.c_str());
        }
        if( available_.empty())
        {
            PANICV( "no buffers available after flush");
        }
        current_ = available_.front();
        available_.pop_front();
    }
    //put the unpushed portion of old current_ onto new one.
    current_->reset();
    writeq_->push( prev.release());
}

void
DestThreadedBufferPriv::empty_wait( MutexGuard &mg )
{
    while( writeq_->size())
    {
        if( puterr_ )
        {
            PANICV( errbuf_.c_str());
        }
        flush_.wait( mg );
    }
    if( puterr_ )
    {
        PANICV( errbuf_.c_str());
    }
}

void
DestThreadedBuffer::flush()
{
    if( d_->current_->currentLength() and
        d_->record_delim_ and
        *(
            d_->current_->get() + 
            d_->current_->currentLength() - 1 
        ) != d_->record_delim_
    ){
        size_t index = d_->current_->currentLength() - 1  ;
        char lastchar = *( d_->current_->get() + index ); 
        char penultchar = *( d_->current_->get() + index - 1 ); 
        char nextchar = *( d_->current_->get() + index + 1 ); 
        PANICV( 
            "I only flush on %x, idx=%lu, last 2+1 bytes: %0x %0x %0x buffer(maybe cut off): %s", 
            static_cast< int >( d_->record_delim_),
            index, 
            static_cast< int >( penultchar),
            static_cast< int >( lastchar), 
            static_cast< int >( nextchar), 
            d_->current_->get());
    }
    if( d_->current_ )
    {
        d_->writeq_->push( d_->current_ );
        d_->current_ = NULL;
    }
    MutexGuard mg(d_->lock_);
    d_->empty_wait( mg );
}

pDest
DestThreadedBuffer::factory( pDest &pd, ConfigPtr &cfg )
{
    //const char *dlm = cfg->getString(DEST_RECORD_SEP);
    //APPLOG_DBG("delim=%x", static_cast<int>(*dlm));

    size_t buffer_size = cfg->getInt( DEST_BUFSIZE, DEST_BUFSIZE_DFLT );
    size_t buffer_count = cfg->getInt( DEST_BUFFERS, DEST_BUFFERS_DFLT );
    bool threaded = cfg->getBool( DEST_THREADED, DEST_THREADED_DFLT );
    char recdelim = * ( cfg->getString(
        DEST_RECORD_SEP, DEST_RECORD_SEP_DFLT ));
    bool buffered = cfg->getBool( DEST_BUFFERED, DEST_BUFFERED_DFLT );

    //dlm = cfg->getString(DEST_RECORD_SEP);
    //APPLOG_DBG("threaded=%i", static_cast<int>(threaded));
    if(!buffered)
    {
        return pd;
    }
    pDest gb( new DestThreadedBuffer( 
        pd, recdelim, buffer_count, buffer_size, threaded ));
    return gb;
}

