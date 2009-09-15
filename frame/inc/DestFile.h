// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestFile.h
// Author:    mgrosso 
// Created:   Mon Jul  8 11:07:13 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: DestFile.h,v 1.4 2004/03/31 00:07:35 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DESTFILE_H
#define DESTFILE_H 1

#include "Dest.h"

class DestFile : public Dest 
{
public:

    DestFile( const char *filename );
    DestFile( const char *filename, bool close_on_delete );
    DestFile( const char *filename, bool close_on_delete, bool exclusive, bool truncate );
    DestFile( int fd );
    DestFile( int fd, bool close_on_delete );
    virtual void put( const char *s );
    virtual void put( const char *s, int slen );
    virtual void flush(void);
    virtual ~DestFile();

private:
    int fd_;
    bool close_on_delete_;

    //not implemented
    DestFile();
    DestFile( const DestFile &rhs );
    DestFile &operator=( const DestFile &rhs );
};

#endif /* DESTFILE_H */

