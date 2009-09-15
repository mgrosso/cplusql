// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Line:      DBMap.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Aug 11 20:42:55 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DBMap.h,v 1.3 2004/09/21 17:56:28 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBMAP_H
#define DBMAP_H 1

#include "db/DBConnection.h"
#include "AbstractDelimitedSourceMap.h"
#include "RelationMeta.h"

class DBMapPriv ;

class DBMap : public AbstractDelimitedSourceMap
{
	protected:
    virtual DelimitedSourcePtr get_( const char *key );
    virtual DelimitedSourcePtr get_( ExpressionList & exp );
    virtual bool exists_( const char * key );
    virtual bool exists_( ExpressionList & list );
    virtual size_t size_();

    public:
    DBMap( RelationMetaPtr& rel, DBConnectionPtr & conn, const char * querytemplate);

    virtual                     ~DBMap();

    //extensions
    virtual const char *    getRelationName()const;
    virtual char_ptr        getRelationHeader()const;
    virtual size_t          getNumColumns()const;
    virtual const char *    getColumnName( size_t index )const;
    virtual size_t          getColumnIndex( const char *col_name )const;

    private:
    DBMapPriv *d_ ;

    //not implemented
    DBMap();
    DBMap( const DBMap &rhs);
    DBMap &operator=( const DBMap &rhs);
};

#endif /* DBMAP_H */
