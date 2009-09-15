
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ThreadedByteSource.h
// Author:    mgrosso 
// Created:   Sat Jul  3 14:03:51 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: ThreadedByteSource.h,v 1.1 2004/07/13 22:46:39 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef THREADEDBYTESOURCE_H
#define THREADEDBYTESOURCE_H 1

#include "CopyByteSource.h"
#include "ByteSource.h"

class ThreadedByteSourcePriv ;
class ThreadedByteSource : public CopyByteSource
{
    public:
                                ThreadedByteSource( 
        ByteSourcePtr &bsp, size_t num_buffers, size_t buffer_size );
    virtual                     ~ThreadedByteSource();
    virtual bool                next( char * &buf, size_t &len );
    
    private:
    ThreadedByteSourcePriv *d_;
    //not implemented
    ThreadedByteSource();
    ThreadedByteSource( const ThreadedByteSource &rhs );
    ThreadedByteSource &operator=( const ThreadedByteSource &rhs );
};



#endif /* THREADEDBYTESOURCE_H */
