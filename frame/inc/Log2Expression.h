
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Log2Expression.h
// Author:    mgrosso 
// Created:   Fri Jan 28 14:40:36 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: Log2Expression.h,v 1.1 2005/02/01 01:24:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LOG2EXPRESSION_H
#define LOG2EXPRESSION_H 1
#include "RangedUnaryArithmeticExpression.h"
    
class Log2ExpressionPriv ;
class Log2Expression : virtual public RangedUnaryArithmeticExpression 
{
    public:
    Log2Expression( ExpressionPtr &operand, ExpressionPtr &ifedom );
    virtual ~Log2Expression();
    virtual long double         getLongDouble(long double op) ;

    private:
    //not implemented
    Log2Expression();
    Log2Expression( const Log2Expression &rhs );
    Log2Expression &operator=( const Log2Expression &rhs );
};



#endif /* LOG2EXPRESSION_H */
