
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RangedUnaryArithmeticExpression.h
// Author:    mgrosso 
// Created:   Sat Sep  4 14:40:10 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: RangedUnaryArithmeticExpression.h,v 1.1 2004/09/21 16:30:57 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef RANGEDUNARYARITHMETICEXPRESSION_H
#define RANGEDUNARYARITHMETICEXPRESSION_H 1

#include "Expression.h"

class RangedUnaryArithmeticExpressionPriv ;
class RangedUnaryArithmeticExpression : public Expression
{
    public:
    RangedUnaryArithmeticExpression( ExpressionPtr &operand, ExpressionPtr &ifedom );
    virtual ~RangedUnaryArithmeticExpression();

    virtual void                notify();

    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:
    RangedUnaryArithmeticExpressionPriv *d_;
    virtual long double         getLongDouble(long double op) =0 ;
    long double                 edom();
    private:
    //not implemented
    RangedUnaryArithmeticExpression();
    RangedUnaryArithmeticExpression( const RangedUnaryArithmeticExpression &rhs );
    RangedUnaryArithmeticExpression &operator=( const RangedUnaryArithmeticExpression &rhs );
};

#endif /* RANGEDUNARYARITHMETICEXPRESSION_H */
