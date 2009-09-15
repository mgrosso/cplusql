// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RandomExpression.h
// Author:    mgrosso 
// Created:   Mon Apr 28 15:19:50 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: RandomExpression.h,v 1.3 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef RANDOMEXPRESSION_H
#define RANDOMEXPRESSION_H 1

#include "ArithmeticExpression.h"

class RandomExpression : public ArithmeticExpression
{
    public:
    RandomExpression( ExpressionPtr &min, ExpressionPtr &max );
    ~RandomExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    private:
    //not implemented
    RandomExpression();
    RandomExpression( const RandomExpression &rhs );
    RandomExpression &operator=( const RandomExpression &rhs );
};

#endif /* RANDOMEXPRESSION_H */
