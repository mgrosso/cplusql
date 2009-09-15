// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LnExpression.h
// Author:    mgrosso 
// Created:   Sat Sep  4 17:44:57 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: LnExpression.h,v 1.2 2005/02/01 01:26:53 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LNEXPRESSION_H
#define LNEXPRESSION_H 1

#include "RangedUnaryArithmeticExpression.h"

class LnExpressionPriv ;
class LnExpression : virtual public RangedUnaryArithmeticExpression 
{
    public:
    LnExpression( ExpressionPtr &operand, ExpressionPtr &ifedom );
    virtual ~LnExpression();
    virtual long double         getLongDouble(long double op) ;

    private:
    //not implemented
    LnExpression();
    LnExpression( const LnExpression &rhs );
    LnExpression &operator=( const LnExpression &rhs );
};

#endif /* LNEXPRESSION_H */

