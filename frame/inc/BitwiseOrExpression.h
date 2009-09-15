
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BitwiseOrExpression.h
// Author:    mgrosso 
// Created:   Sun Nov 21 14:49:38 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: BitwiseOrExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef BITWISEOREXPRESSION_H
#define BITWISEOREXPRESSION_H 1

#include "ArithmeticExpression.h"
    
class BitwiseOrExpression : public ArithmeticExpression 
{
    public:
    BitwiseOrExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    virtual ~BitwiseOrExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    private:
    //not implemented
    BitwiseOrExpression();
    BitwiseOrExpression( const BitwiseOrExpression &rhs );
    BitwiseOrExpression &operator=( const BitwiseOrExpression &rhs );
};



#endif /* BITWISEOREXPRESSION_H */
