#ident "file_id $Id: BitwiseOrExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BitwiseOrExpression.cpp
// Author:    mgrosso 
// Created:   Sun Nov 21 14:49:38 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: BitwiseOrExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BitwiseOrExpression.h"
#include "BinaryExpressionPriv.h"

BitwiseOrExpression::BitwiseOrExpression(
    ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

BitwiseOrExpression::~BitwiseOrExpression()
{
    ;//noop
}

long int
BitwiseOrExpression::getLong() const
{
    return d_->lhs_->getLong() | d_->rhs_->getLong();
}

double              
BitwiseOrExpression::getDouble() const
{
    return getLong();
}

long long
BitwiseOrExpression::getLongLong() const
{
    return d_->lhs_->getLongLong() | d_->rhs_->getLongLong();
}

long double
BitwiseOrExpression::getLongDouble() const
{
    return getLongLong();
}

