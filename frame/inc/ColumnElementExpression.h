// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ColumnElementExpression.h
// Author:    djain Deepak Jain
// Created:   Wed Feb  5 10:44:56 PST 2003 on sf-devdw002.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ColumnElementExpression.h,v 1.5 2004/05/18 04:37:13 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef COLUMNELEMENTEXPRESSION_H
#define COLUMNELEMENTEXPRESSION_H 1

#include "Expression.h"
#include "ParsedColumns.h"
#include "AbstractRelation.h"

class ColumnElementExpressionPriv;

class ColumnElementExpression : public Expression 
{
    public:
    ColumnElementExpression( const ParsedColumnsPtr & col,
			     unsigned int field_index );
    virtual ~ColumnElementExpression();
    virtual void                notify() ;
    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    private:
    ColumnElementExpressionPriv* d_;
    //not implemented
    ColumnElementExpression();
    ColumnElementExpression(const ColumnElementExpression &rhs);
    ColumnElementExpression &operator=(const ColumnElementExpression &rhs);
};

#endif /* COLUMNELEMENTEXPRESSION_H */
