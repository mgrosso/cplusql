
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LongDoubleFormatExpression.h
// Author:    mgrosso 
// Created:   Mon Sep  6 19:41:56 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: LongDoubleFormatExpression.h,v 1.1 2004/09/21 16:09:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LONGDOUBLEFORMATEXPRESSION_H
#define LONGDOUBLEFORMATEXPRESSION_H 1
#include "RangedUnaryArithmeticExpression.h"

class LongDoubleFormatExpressionPriv ;
class LongDoubleFormatExpression : public RangedUnaryArithmeticExpression
{
    public:
    LongDoubleFormatExpression( const char *format, ExpressionPtr &operand );
    virtual ~LongDoubleFormatExpression();
    virtual const char *        getRaw();

    protected:
    virtual long double         getLongDouble(long double op) ;

    private:
    LongDoubleFormatExpressionPriv *ld_;
    //not implemented
    LongDoubleFormatExpression();
    LongDoubleFormatExpression( const LongDoubleFormatExpression &rhs );
    LongDoubleFormatExpression &operator=( const LongDoubleFormatExpression &rhs );
};

#endif /* LONGDOUBLEFORMATEXPRESSION_H */
