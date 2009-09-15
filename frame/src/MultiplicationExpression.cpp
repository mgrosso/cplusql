#ident "file_id $Id: MultiplicationExpression.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MultiplicationExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 19:44:38 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MultiplicationExpression.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MultiplicationExpression.h"
#include "BinaryExpressionPriv.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"

MultiplicationExpression::MultiplicationExpression( ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

MultiplicationExpression::~MultiplicationExpression()
{
    ;//noop
}

long int
MultiplicationExpression::getLong() const
{
    return d_->lhs_->getLong() * d_->rhs_->getLong();
}

double              
MultiplicationExpression::getDouble() const
{
    double d = d_->lhs_->getDouble() * d_->rhs_->getDouble();
	if (d == 0)
	{
		return 0;
	}
	else
	{
		return d;
	}
}

long long
MultiplicationExpression::getLongLong() const
{
    return d_->lhs_->getLongLong() * d_->rhs_->getLongLong();
}

long double
MultiplicationExpression::getLongDouble() const
{
    long double d = d_->lhs_->getLongDouble() * d_->rhs_->getLongDouble();
    if( d==0 )
    {
        return 0;
    }
    else
    {
        return d;
    }
}
