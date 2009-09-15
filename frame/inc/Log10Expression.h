
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Log10Expression.h
// Author:    mgrosso 
// Created:   Fri Jan 28 14:21:23 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: Log10Expression.h,v 1.1 2005/02/01 01:24:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LOG10EXPRESSION_H
#define LOG10EXPRESSION_H 1
#include "RangedUnaryArithmeticExpression.h"

class Log10ExpressionPriv ;
class Log10Expression : virtual public RangedUnaryArithmeticExpression 
{
    public:
    Log10Expression( ExpressionPtr &operand, ExpressionPtr &ifedom );
    virtual ~Log10Expression();
    virtual long double         getLongDouble(long double op) ;

    private:
    //not implemented
    Log10Expression();
    Log10Expression( const Log10Expression &rhs );
    Log10Expression &operator=( const Log10Expression &rhs );
};



#endif /* LOG10EXPRESSION_H */
