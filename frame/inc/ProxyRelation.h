// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyRelation.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Nov 10 00:00:19 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ProxyRelation.h,v 1.4 2005/01/24 18:43:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef PROXYRELATION_H
#define PROXYRELATION_H 1

#include "ptr.h"
#include "AbstractDelimitedSource.h"
#include "AbstractRelation.h"
#include "RelationMeta.h"
#include "Expression.h"

class ProxyRelationPriv ;

class ProxyRelation : virtual public AbstractRelation
{
    protected:
    ProxyRelation(
        RelationMetaPtr &meta,
        ExpressionList &all
    );
    ProxyRelation(
        RelationMetaPtr &meta
    );
    ProxyRelation();
    //virtual bool                childNotify()=0;
            void                addExpression( ExpressionPtr &e );
            void                clearExpressions();

    public:
    virtual                     ~ProxyRelation();
    virtual int                 lineNum()=0;
    virtual char_ptr            getCharPtr();

    //extensions
    virtual const char *        getRaw( size_t index );
    virtual char_ptr            getCharPtr( size_t index )const ;
    virtual bool                getBool( size_t index )const ;
    virtual long int            getLong( size_t index )const ;
    virtual double              getDouble( size_t index )const ;
    virtual long long int       getLongLong( size_t index )const ;
    virtual long double         getLongDouble( size_t index )const ;

    virtual const char *        getRelationName()const ;
    virtual char_ptr            getRelationHeader()const ;
    virtual size_t              getNumColumns()const ;
    virtual const char *        getColumnName( size_t index )const ;
    virtual size_t              getColumnIndex( const char *col_name )const ;

    private:
    ProxyRelationPriv *proxypriv_;

    //not implemented
    ProxyRelation( const ProxyRelation &rhs);
    ProxyRelation &operator=( const ProxyRelation &rhs);
};

#endif /* PROXYRELATION_H */
