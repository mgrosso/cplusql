#ident "file_id $Id: BitwiseAndExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BitwiseAndExpression.cpp
// Author:    mgrosso 
// Created:   Sun Nov 21 14:40:36 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: BitwiseAndExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BitwiseAndExpression.h"
#include "BinaryExpressionPriv.h"

BitwiseAndExpression::BitwiseAndExpression(
    ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

BitwiseAndExpression::~BitwiseAndExpression()
{
    ;//noop
}

long int
BitwiseAndExpression::getLong() const
{
    return d_->lhs_->getLong() & d_->rhs_->getLong();
}

double              
BitwiseAndExpression::getDouble() const
{
    return getLong();
}

long long
BitwiseAndExpression::getLongLong() const
{
    return d_->lhs_->getLongLong() & d_->rhs_->getLongLong();
}

long double
BitwiseAndExpression::getLongDouble() const
{
    return getLongLong();
}

