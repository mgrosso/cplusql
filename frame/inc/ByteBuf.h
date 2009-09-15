
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ByteBuf.h
// Author:    mgrosso 
// Created:   Wed May 26 03:51:06 EDT 2004 on caliban
// Project:   
// Purpose:   dynamically growable place to put bytes.
// 
// $Id: ByteBuf.h,v 1.5 2005/01/24 18:31:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef BYTEBUF_H
#define BYTEBUF_H 1

#include <sys/types.h>
#include "ptr.h"
    
class ByteBufPriv ;
class ByteBuf
{
    public:
    ByteBuf();// size_t initial=8, size_t maxstep=4096 
    ByteBuf( size_t initial, size_t maxstep );
    char *                      rewind();
    char *                      reset();
    char *                      reset( size_t newsize ) ;
    char *                      reset( const char *s );
    char *                      reset( const char *s, size_t len );
    void                        shrink( size_t newsize ) ;
    void                        append( char c );
    char *                      append( const char *s );
    char *                      append( const char *s, size_t len );
    char *                      appendNoNull( const char *s );
    size_t                      currentLength() const ;
    size_t                      bytesUsed() const ;
    bool                        hasRoom( size_t strlength ) const ;
    void                        ensureSpace( size_t newsize ) ;
    char *                      buf();
    const char *                get() const;
    ~ByteBuf();

    private:

    ByteBufPriv *d_;
    //not implemented
    ByteBuf( const ByteBuf &rhs );
    ByteBuf &operator=( const ByteBuf &rhs );
};

typedef ptr< ByteBuf > ByteBufPtr ;

#endif /* BYTEBUF_H */
