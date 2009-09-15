
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PgCopyByteSource.h
// Author:    mgrosso 
// Created:   Wed Nov 17 22:07:13 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: PgCopyByteSource.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PGCOPYBYTESOURCE_H
#define PGCOPYBYTESOURCE_H 1

#include "db/DBConnection.h"
#include "ByteSource.h"

class PgCopyByteSourcePriv ;
class PgCopyByteSource : public ByteSource
{
public:
    PgCopyByteSource( DBConnectionPtr conn, const char *copy_statement );

    virtual ~PgCopyByteSource();
    virtual bool next( char *buffer, size_t &bytes_read, size_t bufsize );

private:
    PgCopyByteSourcePriv *d_;
    //not implemented
    PgCopyByteSource();
    PgCopyByteSource( const PgCopyByteSource &rhs );
    PgCopyByteSource &operator=( const PgCopyByteSource &rhs );
};



#endif /* PGCOPYBYTESOURCE_H */
