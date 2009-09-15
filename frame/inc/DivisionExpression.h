// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DivisionExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:32:05 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DivisionExpression.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DIVISIONEXPRESSION_H
#define DIVISIONEXPRESSION_H 1

#include "Expression.h"
#include "ArithmeticExpression.h"

class DivisionExpression : public ArithmeticExpression 
{
    public:
    DivisionExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    virtual ~DivisionExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:

    private:
    //not implemented
    DivisionExpression();
    DivisionExpression(const DivisionExpression &rhs);
    DivisionExpression &operator=(const DivisionExpression &rhs);
};

#endif /* DIVISIONEXPRESSION_H */
