// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ArithmeticExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:31:30 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ArithmeticExpression.h,v 1.5 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ARITHMETICEXPRESSION_H
#define ARITHMETICEXPRESSION_H 1

#include "Expression.h"

class BinaryExpressionPriv  ;

class ArithmeticExpression : public Expression 
{
    public:
    virtual ~ArithmeticExpression();
    virtual void                notify() ;
    virtual void                childNotify() ;

    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const =0 ;
    virtual double              getDouble() const =0 ;
    virtual long long           getLongLong() const =0 ;
    virtual long double         getLongDouble() const =0;

    protected:
    ArithmeticExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    BinaryExpressionPriv *d_;
    private:
    //not implemented
    ArithmeticExpression();
    ArithmeticExpression(const ArithmeticExpression &rhs);
    ArithmeticExpression &operator=(const ArithmeticExpression &rhs);
};

#endif /* ARITHMETICEXPRESSION_H */
