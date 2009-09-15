// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LiteralExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:08:07 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LiteralExpression.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef LITERALEXPRESSION_H
#define LITERALEXPRESSION_H 1

#include "Expression.h"

class LiteralExpressionPriv  ;

class LiteralExpression : public Expression 
{
    public:
    LiteralExpression(const char *value );
    virtual ~LiteralExpression();
    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    //explicitly change value
    void                        set( const char *new_value );
    void                        take( char *v );
    protected:

    private:
    char *val_;
    //not implemented
    LiteralExpression();
    LiteralExpression(const LiteralExpression &rhs);
    LiteralExpression &operator=(const LiteralExpression &rhs);
};

#endif /* LITERALEXPRESSION_H */
