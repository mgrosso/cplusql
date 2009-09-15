#ident "file_id $Id: AdditionExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AdditionExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 19:28:10 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AdditionExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AdditionExpression.h"
#include "BinaryExpressionPriv.h"
#include "Convert.h"

AdditionExpression::AdditionExpression( ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

AdditionExpression::~AdditionExpression()
{
    ;//noop
}

long int
AdditionExpression::getLong() const
{
    return d_->lhs_->getLong() + d_->rhs_->getLong();
}

double              
AdditionExpression::getDouble() const
{
    return d_->lhs_->getDouble() + d_->rhs_->getDouble();
}

long long
AdditionExpression::getLongLong() const
{
    return d_->lhs_->getLongLong() + d_->rhs_->getLongLong();
}

long double
AdditionExpression::getLongDouble() const
{
    return d_->lhs_->getLongDouble() + d_->rhs_->getLongDouble();
}


