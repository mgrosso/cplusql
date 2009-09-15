#ident "file_id $Id: PowExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PowExpression.cpp
// Author:    mgrosso 
// Created:   Sun Nov 21 15:00:36 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: PowExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "PowExpression.h"
#include "BinaryExpressionPriv.h"
#include <math.h>

PowExpression::PowExpression(
    ExpressionPtr &lhs, ExpressionPtr &rhs, ExpressionPtr &edom )
:ArithmeticExpression(lhs,rhs),edom_(edom)
{
    ;//noop
}

PowExpression::~PowExpression()
{
    ;//noop
}

long int
PowExpression::getLong() const
{
    return static_cast< long int> (getLongDouble());
}

double              
PowExpression::getDouble() const
{
    return static_cast< double >( getLongDouble());
}

long long
PowExpression::getLongLong() const
{
    return static_cast< long long >( getLongDouble());
}

long double
PowExpression::getLongDouble() const
{
    long double x = d_->lhs_->getLongDouble(); 
    long double y = d_->rhs_->getLongDouble();
    long double ret = powl( x, y );
    //if x < 0 and y is not an integral, we should get EDOM ...
    if( errno == EDOM )
    {
        errno = 0;
        Expression *errhandler = const_cast<Expression *>( edom_.get());
        errhandler->notify();
        return errhandler->getLongDouble();
    }
    return ret;
}

