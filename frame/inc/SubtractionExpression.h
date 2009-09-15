// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SubtractionExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:31:50 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SubtractionExpression.h,v 1.3 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SUBTRACTIONEXPRESSION_H
#define SUBTRACTIONEXPRESSION_H 1

#include "ArithmeticExpression.h"

class SubtractionExpressionPriv  ;

class SubtractionExpression : public ArithmeticExpression 
{
    public:
    SubtractionExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    virtual ~SubtractionExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:

    private:
    //not implemented
    SubtractionExpression();
    SubtractionExpression(const SubtractionExpression &rhs);
    SubtractionExpression &operator=(const SubtractionExpression &rhs);
};

#endif /* SUBTRACTIONEXPRESSION_H */
