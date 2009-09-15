
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestPgCopy.h
// Author:    mgrosso 
// Created:   Sat May 29 16:03:44 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: DestPgCopy.h,v 1.2 2005/03/11 19:46:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DESTPGCOPY_H
#define DESTPGCOPY_H 1

#include "Dest.h"
#include "db/DBConnection.h"

class DestPgCopyPriv ;
class DestPgCopy : public Dest 
{
    public:
    DestPgCopy( 
        DBConnectionPtr &conn, int maxcache, const char *tablename, bool blank_is_null=true );
    virtual ~DestPgCopy();
    virtual void put( const char *s );
    virtual void put( const char *s, int slen );
    virtual void flush(void);
            void finish(void);

    private:
    DestPgCopyPriv *d_;
    //not implemented
    DestPgCopy();
    DestPgCopy( const DestPgCopy &rhs );
    DestPgCopy &operator=( const DestPgCopy &rhs );
};

#endif /* DESTPGCOPY_H */
