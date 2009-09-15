#ident "file_id $Id: RandomExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RandomExpression.cpp
// Author:    mgrosso 
// Created:   Mon Apr 28 15:33:21 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: RandomExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RandomExpression.h"
#include "BinaryExpressionPriv.h"
#include "Convert.h"
#include "BaseException.h"
#include "Random.h"

RandomExpression::RandomExpression( ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

RandomExpression::~RandomExpression()
{
    ;//noop
}

long int
RandomExpression::getLong() const
{
    return static_cast<long int> (getDouble());
}

double              
RandomExpression::getDouble() const
{
    double min = d_->lhs_->getDouble();
    double max = d_->rhs_->getDouble();
    return Random::nextValue( min, max );
}

long long
RandomExpression::getLongLong() const
{
    return static_cast<long long int> (getDouble());
}

long double
RandomExpression::getLongDouble() const
{
    return static_cast<long double> (getDouble());
}

