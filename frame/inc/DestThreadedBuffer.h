
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestThreadedBuffer.h
// Author:    mgrosso 
// Created:   Tue Jun 22 09:22:05 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: DestThreadedBuffer.h,v 1.4 2004/06/30 12:14:06 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DESTTHREADEDBUFFER_H
#define DESTTHREADEDBUFFER_H 1

#include "Dest.h"
#include "frame_keys.h"
#include "Config.h"

class DestThreadedBufferPriv ;
class DestThreadedBuffer : public Dest
{
    public:
    DestThreadedBuffer(
        pDest &dest,
        char record_delim,
        size_t buffer_count,
        size_t buffer_size,
        bool use_threads
    );
    virtual ~DestThreadedBuffer();

    virtual void put( const char *s );
    virtual void put( const char *s, int slen );
    virtual void flush(void);

    //convenience helper 
    static pDest factory( pDest &dest, ConfigPtr &cfg );

    private:
    DestThreadedBufferPriv *d_;
    //not implemented
    DestThreadedBuffer();
    DestThreadedBuffer( const DestThreadedBuffer &rhs );
    DestThreadedBuffer &operator=( const DestThreadedBuffer &rhs );
};



#endif /* DESTTHREADEDBUFFER_H */
