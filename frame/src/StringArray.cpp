#ident "file_id $Id: StringArray.cpp,v 1.4 2004/12/31 21:49:10 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArray.cpp
// Author:    mgrosso 
// Created:   Wed Jul  7 20:17:40 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArray.cpp,v 1.4 2004/12/31 21:49:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <vector>
#include <string.h>
#include "StringArray.h"

#include "ByteBuf.h"
#include "BaseException.h"
#include "AppLog.h"

#define BUFINCR 32768

class StringArrayPriv 
{
    private:
    friend class StringArray ;
    vector< char * >        v_;
    vector< char * >        take_ ;
    list< ByteBuf * >       copy_ ;
    bool                    leak_ ;
    void    copy( const char *s );
    ~StringArrayPriv();
};

void
StringArrayPriv::copy( const char *s )
{
    size_t sz = strlen(s);
    ByteBuf *b = copy_.front();
    if( ! b->hasRoom( sz ))
    {
        //APPLOG_DBG("alloc new ByteBuf() bytes used=%lu, current length=%lu appending sz=%lu",
        //    b->bytesUsed(), b->currentLength(), sz + 1 );
        b = new ByteBuf( BUFINCR, BUFINCR );
        copy_.push_front(b);
    }
    v_.push_back( b->append( s ));
}

StringArray::StringArray( bool leak )
{
    init(leak);
}

StringArray::StringArray()
{
    init(false);
}

void
StringArray::init( bool leak )
{
    d_=new StringArrayPriv ();
    ByteBuf *b = new ByteBuf( BUFINCR, BUFINCR );
    d_->copy_.push_front(b);
    d_->leak_ = leak ;
}

StringArrayPriv::~StringArrayPriv()
{
    for( size_t i=0; i< take_.size(); ++i )
    {
        delete [] take_[i];
    }
    while( ! copy_.empty())
    {
        //APPLOG_DBG( "copy_ ByteBuf * %lx", reinterpret_cast< unsigned long> (copy_.front()));
        delete copy_.front();
        copy_.pop_front();
    }
}

StringArray::~StringArray()
{
    if( d_->leak_ )
    {
        return ;
    }
    delete d_;
}

void
StringArray::take( char *s )
{
    if( ! s )
    {
        PANICV("cannot take null pointer.");
    }
    d_->v_.push_back( s );
    d_->take_.push_back( s );
}

void
StringArray::take( char_ptr &s )
{
    char *ss = s.release();
    if(!ss)
    {
        PANICV("cant give me what you dont own. could not release ptr.");
    }
    d_->v_.push_back( ss );
    d_->take_.push_back( ss );
}

void
StringArray::take( list< char_ptr > & s )
{
    list< char_ptr >::iterator i = s.begin();
    list< char_ptr >::iterator e = s.end();
    for( ; i!=e; ++i )
    {
        take( (*i).release() );
    }
}

void
StringArray::copy( const char *s )
{
    //APPLOG_DBG("s=%s", s );
    d_->copy( s );
}

void
StringArray::copy( const char_ptr &s )
{
    d_->copy( s.get());
}

void
StringArray::copy( const list< char_ptr > & s )
{
    list< char_ptr >::const_iterator i = s.begin();
    list< char_ptr >::const_iterator e = s.end();
    for( ; i!=e; ++i )
    {
        d_->copy( (*i).get());
    }
}

char *
StringArray::get( size_t index )
{
    return d_->v_[index];
}

const char *
StringArray::get( size_t index ) const
{
    return d_->v_[index];
}

size_t
StringArray::size() const
{
    return d_->v_.size();
}

