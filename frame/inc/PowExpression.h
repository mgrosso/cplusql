
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PowExpression.h
// Author:    mgrosso 
// Created:   Sun Nov 21 15:00:36 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: PowExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef POWEXPRESSION_H
#define POWEXPRESSION_H 1

#include "ArithmeticExpression.h"

class PowExpressionPriv ;
class PowExpression: public ArithmeticExpression 
{
    public:
    PowExpression( ExpressionPtr &lhs, ExpressionPtr &rhs, 
        ExpressionPtr &edom );
    virtual ~PowExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    private:
    ExpressionPtr edom_;
    //not implemented
    PowExpression();
    PowExpression( const PowExpression &rhs );
    PowExpression &operator=( const PowExpression &rhs );
};



#endif /* POWEXPRESSION_H */
