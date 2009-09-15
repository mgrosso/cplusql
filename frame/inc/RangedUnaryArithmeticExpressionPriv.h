// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RangedUnaryArithmeticExpressionPriv.h
// Author:    mgrosso 
// Created:   Mon Sep  6 20:26:13 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: RangedUnaryArithmeticExpressionPriv.h,v 1.1 2004/09/21 16:30:56 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef RANGEDUNARYARITHMETICEXPRESSIONPRIV_H
#define RANGEDUNARYARITHMETICEXPRESSIONPRIV_H 1
#include "RangedUnaryArithmeticExpression.h"

class RangedUnaryArithmeticExpressionPriv 
{
    public: 
    long double                 value_ ;
    ExpressionPtr               operand_ ;
    ExpressionPtr               ifedom_ ;
    char                        raw_[64];
    RangedUnaryArithmeticExpressionPriv( ExpressionPtr &operand, ExpressionPtr &ifedom )
    :value_(0),operand_(operand), ifedom_(ifedom)
    {
        raw_[0]='\0';
    };
};

#endif /* RANGEDUNARYARITHMETICEXPRESSIONPRIV_H */

