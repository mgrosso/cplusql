#ident "file_id $Id: ByteBuf.cpp,v 1.10 2005/01/24 18:31:15 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ByteBuf.cpp
// Author:    mgrosso 
// Created:   Wed May 26 03:51:06 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: ByteBuf.cpp,v 1.10 2005/01/24 18:31:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>

#include "AppLog.h"
#include "ByteBuf.h"
#include "ptr_strdup.h"

//void
//mymemcpy( void *dest, const void *src, size_t len )
//{
//    unsigned char *d = static_cast< unsigned char *>(dest);
//    const unsigned char *s = static_cast< const unsigned char *>(src);
//    for( size_t i=0; i<len; ++i )
//    {
//        d[i] = s[i];
//    }
//};

#if 0
#define DBG_BYTEBUF_C(x) \
    APPLOG_DBG( "this=%lx cursor_=%lx start_=%lx end_=%lx size=%u cursor=%u end=%u c=%lx",  \
        reinterpret_cast< long unsigned int >(this),  \
        reinterpret_cast< long unsigned int >(cursor_), \
        reinterpret_cast< long unsigned int >(start_), \
        reinterpret_cast< long unsigned int >(end_), \
        size_, cursor_ - start_, end_ - start_, (unsigned long)x )
#define DBG_BYTEBUF() \
    APPLOG_DBG( "this=%lx cursor_=%lx start_=%lx end_=%lx size=%u cursor=%u end=%u",  \
        reinterpret_cast< long unsigned int >(this),  \
        reinterpret_cast< long unsigned int >(cursor_), \
        reinterpret_cast< long unsigned int >(start_), \
        reinterpret_cast< long unsigned int >(end_), \
        size_, cursor_ - start_, end_ - start_ )
#else
#define DBG_BYTEBUF_C(x) 
#define DBG_BYTEBUF()
#endif


class ByteBufPriv 
{
    private:
    friend class ByteBuf ;
    size_t                      size_ ;
    size_t                      maxstep_ ; char_ptr                    bytes_ ;
    char *                      start_ ;
    char *                      cursor_ ;
    char *                      end_ ;

                                ByteBufPriv(size_t initial, size_t maxstep);
    inline void                 ensureSpace( size_t delta );
    inline bool                 hasSpace( size_t delta )const; 
    inline void                 setSize( size_t bytes );
    inline void                 put( char c );
    inline char *               put( const char *s );
    inline char *               put( const char *s, size_t len );
    inline void                 reset( const char *s );
    inline void                 reset();
    inline void                 rewind();
};

void
ByteBufPriv::put( char c )
{
    DBG_BYTEBUF_C(c);
    *cursor_++ = c;
    //for( int x = 0; x < end_ - start_ +1 ; ++x )
    //{
    //    APPLOG_DBG( "before dump: %i = %lx, %c", 
    //        x, (unsigned long)start_[x], start_[x] );
    //}
    if( cursor_ >= end_ )
    {
        DBG_BYTEBUF_C(c);
        ensureSpace( 1 );
        DBG_BYTEBUF_C(c);
    }
    DBG_BYTEBUF_C(c);
    //for( int x = 0; x < end_ - start_ +1 ; ++x )
    //{
    //    APPLOG_DBG( "after dump: %i = %lx, %c", 
    //        x, (unsigned long)start_[x], start_[x] );
    //}
}

char *
ByteBufPriv::put( const char *s )
{
    char c;
    char *giveback = cursor_ ;
    while(( c = *s++ ))
    {
        put(c);
    }
    return giveback;
}

char *
ByteBufPriv::put( const char *s, size_t len )
{
    ensureSpace( len );
    char *giveback = cursor_ ;
    memcpy( cursor_, s, len );
    cursor_ += len;
    return giveback;
}

ByteBufPriv::ByteBufPriv( size_t initial, size_t maxstep )
:size_(initial), maxstep_(maxstep)
{
    start_ = new char[ size_ ];
    bytes_ = start_ ;
    end_ = start_ + size_ - 1 ;
    rewind();
    DBG_BYTEBUF();
};

void
ByteBufPriv::setSize( size_t bytes )
{
    DBG_BYTEBUF_C(bytes);
    if( bytes <= size_ )
    {
        return;
    }
    char *newbuf = new char[ bytes ];
    memset( newbuf , '\0', bytes );
    memcpy( newbuf, start_, size_ );
    cursor_ = newbuf + ( cursor_ - start_ );
    size_ = bytes ;
    end_=newbuf+size_-1;
    bytes_=newbuf;//will delete start_
    start_=newbuf;

    DBG_BYTEBUF_C(bytes);
}

bool
ByteBufPriv::hasSpace( size_t delta ) const 
{
    if( cursor_ + delta <= end_ )
    {
        return true;
    }
    return false;
}

void
ByteBufPriv::ensureSpace( size_t delta )
{
    DBG_BYTEBUF_C(delta);
    //for( int x = 0; x < end_ - start_ +1 ; ++x )
    //{
        //APPLOG_DBG( "dump: %i = %lx, %c", 
        //    x, (unsigned long)start_[x], start_[x] );
    //}
#if 0
    //size_t lack = delta - (end_ - cursor_ );
    if( cursor_ + delta < end_ )
    {
        return;
    }
    size_t lack = cursor_ + delta - end_ ;
    size_t add = 0;
    if( lack > 0 )
    {
        add = ( size_ > maxstep_ ) ? maxstep_ : 2 * size_ ;
        while( add < delta )
        {
            add += maxstep_ ;
        }
        addSpace( add );
    }
#endif /* 0 */

    if( hasSpace(delta))
    {
        return;
    }
    size_t newsize ;
    for(
        newsize = size_ ;
        newsize < cursor_ - start_ + delta ;
        newsize *= 2 )
    {
        //noop. the loop itself has the side effect of setting newsize correctly.
        //APPLOG_DBG( "this=%lx newsize=%u size_=%u (size_ + delta)=%u", 
        //    reinterpret_cast< long unsigned int >(this), 
        //    newsize, size_, (size_+delta));
    }
    setSize( newsize );
    //APPLOG_DBG( "this=%lx size=%u delta=%u cursor=%u end=%u", 
    //    reinterpret_cast< long unsigned int >(this), 
    //    size_, delta, cursor_ - start_, end_ - start_ );
    DBG_BYTEBUF_C(delta);
    //for( int x = 0; x < end_ - start_ +1 ; ++x )
    //{
    //    //APPLOG_DBG( "dump: %i = %lx, %c", 
    //    //    x, (unsigned long)start_[x], start_[x] );
    //}
}

void
ByteBufPriv::reset()
{
    rewind();
    memset( start_, '\0', size_ );
}

void
ByteBufPriv::rewind()
{
    cursor_ = start_ ;
    *cursor_ = '\0';
}

ByteBuf::ByteBuf( size_t initial, size_t maxstep )
{
    d_=new ByteBufPriv ( initial, maxstep );
}

ByteBuf::ByteBuf()
{
    d_=new ByteBufPriv ( 8, 4096 );
}

ByteBuf::~ByteBuf()
{
    //APPLOG_DBG( "this=%lx, d_=%lx, d_->start_=%lx",
    //    reinterpret_cast< long unsigned int >(this), 
    //    reinterpret_cast< long unsigned int >(d_), 
    //    reinterpret_cast< long unsigned int >(d_->start_));
    delete d_;
}

char *
ByteBuf::rewind()
{
    d_->rewind();
    return d_->start_ ;
}

char *
ByteBuf::reset()
{
    d_->reset();
    return d_->start_ ;
}

void
ByteBuf::append( char c )
{
    d_->put(c);
}

char *
ByteBuf::appendNoNull( const char *s )
{
    return d_->put(s);
}

char *
ByteBuf::append( const char *s )
{
    char *giveback = d_->put(s);
    d_->put('\0');
    return giveback ;
}

char *
ByteBuf::append( const char *s, size_t len )
{
    //APPLOG_DBG( "this=%lx size=%u len=%u cursor=%u end=%u", 
    //    reinterpret_cast< long unsigned int >(d_), 
    //    d_->size_, len, d_->cursor_ - d_->start_, d_->end_ - d_->start_ );

    return d_->put(s, len );
}

char *
ByteBuf::reset( const char *s )
{
    d_->reset();
    d_->put(s);
    d_->put('\0');
    return d_->start_ ;
}

char *
ByteBuf::reset( size_t sz )
{
    d_->reset();
    d_->ensureSpace( sz );
    * d_->start_ = '\0';
    return d_->start_ ;
}

char *
ByteBuf::reset( const char *s, size_t len )
{
    //APPLOG_DBG( "this=%lx size=%u len=%u cursor=%u end=%u", 
    //    reinterpret_cast< long unsigned int >(d_), 
    //    d_->size_, len, d_->cursor_ - d_->start_, d_->end_ - d_->start_ );

    d_->rewind();
    d_->put(s, len );

    //APPLOG_DBG( "this=%lx size=%u len=%u cursor=%u end=%u", 
    //    reinterpret_cast< long unsigned int >(d_), 
    //    d_->size_, len, d_->cursor_ - d_->start_, d_->end_ - d_->start_ );

    return d_->start_ ;
}

void
ByteBuf::shrink( size_t newsize )
{
    if( newsize > bytesUsed())
    {
        ensureSpace(newsize - bytesUsed());
    }
    d_->cursor_ = d_->start_ + newsize ;
}

void
ByteBuf::ensureSpace( size_t newsize )
{
    d_->ensureSpace(newsize );
}

const char *
ByteBuf::get() const
{
    return d_->start_ ;
}

char *
ByteBuf::buf()
{
    return d_->start_ ;
}

size_t
ByteBuf::bytesUsed() const
{
    return d_->size_ ;
}

bool
ByteBuf::hasRoom(size_t strlength) const 
{
    //one extra byte for null. one more because bug prevents use of 
    //last byte.
    return d_->hasSpace( strlength +2 );
}

size_t
ByteBuf::currentLength() const
{
    return d_->cursor_ - d_->start_ ;
}

