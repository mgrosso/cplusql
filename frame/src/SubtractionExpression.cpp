#ident "file_id $Id: SubtractionExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SubtractionExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 19:40:08 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SubtractionExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SubtractionExpression.h"
#include "BinaryExpressionPriv.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"

SubtractionExpression::SubtractionExpression( ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

SubtractionExpression::~SubtractionExpression()
{
    ;//noop
}

long int
SubtractionExpression::getLong() const
{
    return d_->lhs_->getLong() - d_->rhs_->getLong();
}

double              
SubtractionExpression::getDouble() const
{
    return d_->lhs_->getDouble() - d_->rhs_->getDouble();
}

long long
SubtractionExpression::getLongLong() const
{
    return d_->lhs_->getLongLong() - d_->rhs_->getLongLong();
}

long double
SubtractionExpression::getLongDouble() const
{
    return d_->lhs_->getLongDouble() - d_->rhs_->getLongDouble();
}

