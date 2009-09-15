// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ByteSource.h
// Author:    mgrosso 
// Created:   Sat Apr 26 19:55:30 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ByteSource.h,v 1.3 2003/12/12 10:56:42 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef BYTESOURCE_H
#define BYTESOURCE_H 1

#include "ptr.h"

class ByteSource
{
    public:
    virtual ~ByteSource();
    virtual bool next( char *buffer, size_t &bytes_read, size_t bufsize )=0;

    protected:
    ByteSource();

    //allow c++ defaults so children can easily decide whether or not to implement.
    //ByteSource( const ByteSource &rhs );
    //ByteSource &operator=( const ByteSource &rhs );
    private:
};

typedef ptr< ByteSource > ByteSourcePtr ;

#endif /* BYTESOURCE_H */
