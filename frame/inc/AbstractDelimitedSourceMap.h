// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractDelimitedSourceMap.h
// Author:    djain 
// Created:   Fri Dec 19 12:44:45 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractDelimitedSourceMap.h,v 1.3 2004/09/20 20:52:04 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ABSTRACTDELIMITEDSOURCEMAP_H
#define ABSTRACTDELIMITEDSOURCEMAP_H 1

#include <map>
#include "hashFuncs.h"
#include "AbstractDelimitedSource.h"
#include "AbstractRelationMeta.h"
#include "Expression.h"

class ADSMPriv;

class AbstractDelimitedSourceMap : public AbstractRelationMeta
{
	protected:
	AbstractDelimitedSourceMap();
    virtual DelimitedSourcePtr get_( const char *key ) = 0;
    virtual DelimitedSourcePtr get_( ExpressionList & list ) = 0;
    virtual bool exists_( const char * key ) = 0;
    virtual bool exists_( ExpressionList & list ) = 0;
    virtual size_t size_() = 0;

	public:
    virtual ~AbstractDelimitedSourceMap();
    DelimitedSourcePtr get( const char *key );
    DelimitedSourcePtr get( ExpressionPtr & exp);
    DelimitedSourcePtr get( ExpressionList & list);
    bool exists( const char * key );
    bool exists( ExpressionPtr &key );
    bool exists( ExpressionList & list );
    size_t size();

    //extensions
    virtual const char *    getRelationName()const =0;
    virtual size_t          getNumColumns()const =0;
    virtual const char *    getColumnName( size_t index )const =0;
    virtual size_t          getColumnIndex( const char *col_name )const =0;

    private:
	ADSMPriv * d_;
    //not implemented
    AbstractDelimitedSourceMap( const AbstractDelimitedSourceMap &rhs );
    AbstractDelimitedSourceMap &operator=( const AbstractDelimitedSourceMap &rhs );
};

typedef ptr<AbstractDelimitedSourceMap> AbstractDelimitedSourceMapPtr;
#endif /* ABSTRACTDELIMITEDSOURCEMAP_H */
