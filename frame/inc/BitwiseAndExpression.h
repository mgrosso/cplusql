
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BitwiseAndExpression.h
// Author:    mgrosso 
// Created:   Sun Nov 21 14:40:36 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: BitwiseAndExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef BITWISEANDEXPRESSION_H
#define BITWISEANDEXPRESSION_H 1

#include "ArithmeticExpression.h"

class BitwiseAndExpression : public ArithmeticExpression 
{
    public:
    BitwiseAndExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    virtual ~BitwiseAndExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;
    private:
    //not implemented
    BitwiseAndExpression();
    BitwiseAndExpression( const BitwiseAndExpression &rhs );
    BitwiseAndExpression &operator=( const BitwiseAndExpression &rhs );
};

#endif /* BITWISEANDEXPRESSION_H */
