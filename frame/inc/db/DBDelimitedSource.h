// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Line:      DBDelimitedSource.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Aug 11 20:42:55 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DBDelimitedSource.h,v 1.2 2005/01/24 19:09:39 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBDELIMITEDSOURCE_H
#define DBDELIMITEDSOURCE_H 1

#include <stdio.h>
#include "ptr.h"
#include "ptr_strdup.h"
#include "AbstractDelimitedSourceNext.h"
#include "db/DBConnection.h"
#include "db/DBStatement.h"
#include "db/DBResultSet.h"

class DBDelimitedSourcePriv ;

class DBDelimitedSource : virtual public AbstractDelimitedSourceNext
{
    protected:
    DBDelimitedSource();
    DBDelimitedSourcePriv *d_ ;
    public:
    DBDelimitedSource( DBConnectionPtr & connection, const char * query );
    DBDelimitedSource( StatementPtr & stmt, ResultSetPtr & rs );

    virtual                     ~DBDelimitedSource();
    virtual bool                next();
    virtual int                 lineNum() ;
    virtual size_t              getNumColumns() const ;
    virtual const char *        getRaw( size_t index ) ;
    const char *                getRaw( size_t index ) const ;
    virtual char_ptr            getCharPtr( size_t index ) const ;
    bool                        getBool( size_t index ) const ;
    long                        getLong( size_t index ) const ;
    double                      getDouble( size_t index ) const ;
    long long                   getLongLong( size_t index ) const ;
    long double                 getLongDouble( size_t index ) const ;

    private:

    //not implemented
    DBDelimitedSource( const DBDelimitedSource &rhs);
    DBDelimitedSource &operator=( const DBDelimitedSource &rhs);
};

#endif /* DBDELIMITEDSOURCE_H */
