// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ColumnExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:08:32 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ColumnExpression.h,v 1.3 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef COLUMNEXPRESSION_H
#define COLUMNEXPRESSION_H 1

#include "Expression.h"
#include "ptr.h"
#include "AbstractRelation.h"
#include "AbstractDelimitedSource.h"

class ColumnExpressionPriv  ;

class ColumnExpression : public Expression
{
    public:
    ColumnExpression( const DelimitedSourcePtr &dsp, size_t column );
    ColumnExpression( RelationPtr &dsp, size_t column );
    virtual ~ColumnExpression();
    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:

    private:
    ColumnExpressionPriv *d_;
    //not implemented
    ColumnExpression();
    ColumnExpression(const ColumnExpression &rhs);
    ColumnExpression &operator=(const ColumnExpression &rhs);
};

#endif /* COLUMNEXPRESSION_H */
