// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MultiplicationExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:31:57 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MultiplicationExpression.h,v 1.3 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MULTIPLICATIONEXPRESSION_H
#define MULTIPLICATIONEXPRESSION_H 1

#include "ArithmeticExpression.h"

class MultiplicationExpressionPriv  ;

class MultiplicationExpression : public ArithmeticExpression 
{
    public:
    MultiplicationExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    virtual ~MultiplicationExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:

    private:
    //not implemented
    MultiplicationExpression();
    MultiplicationExpression(const MultiplicationExpression &rhs);
    MultiplicationExpression &operator=(const MultiplicationExpression &rhs);
};

#endif /* MULTIPLICATIONEXPRESSION_H */
