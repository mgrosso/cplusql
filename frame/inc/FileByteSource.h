
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FileByteSource.h
// Author:    mgrosso 
// Created:   Sun Apr 27 01:51:06 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: FileByteSource.h,v 1.6 2004/08/11 21:42:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef FILEBYTESOURCE_H
#define FILEBYTESOURCE_H 1

#include "ByteSource.h"
#include "ExecWrap.h"

//need size_t
#include <sys/types.h>

class FileByteSourcePriv ; 

class FileByteSource : public ByteSource
{
    public:
    FileByteSource( const char *filename );
    FileByteSource( int fd );
    FileByteSource( int fd, bool close_on_delete = false );
    FileByteSource( int fd, ewproc_t *ew );//ew not deleted.
    virtual ~FileByteSource();

    virtual bool next( char *buffer, size_t &bytes_read, size_t bufsize );

    private:

    FileByteSourcePriv *d_;

    //not implemented
    FileByteSource();
    FileByteSource( const FileByteSource &rhs );
    FileByteSource &operator=( const FileByteSource &rhs );
};

#endif /* FILEBYTESOURCE_H */
