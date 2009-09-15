
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MultiByteSource.h
// Author:    mgrosso 
// Created:   Sun Apr 27 17:32:20 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: MultiByteSource.h,v 1.4 2003/12/12 10:56:42 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef MULTIBYTESOURCE_H
#define MULTIBYTESOURCE_H 1

#include "ByteSource.h"
#include "ptr_strdup.h"

class MultiByteSourcePriv ;
class MultiByteSource : public ByteSource
{
    public:
    MultiByteSource();
    ~MultiByteSource();
    virtual bool            next( char *buffer, size_t &bytes_read, size_t bufsize );
    void                    add( ByteSourcePtr &bsp );
    void                    add( list<char_ptr> &patterns);
    static ByteSourcePtr    makeFromGlobs( list<char_ptr> &patterns);

    private:
    MultiByteSourcePriv *d_;
    //not implemented
    MultiByteSource( const MultiByteSource &rhs );
    MultiByteSource &operator=( const MultiByteSource &rhs );
};



#endif /* MULTIBYTESOURCE_H */
