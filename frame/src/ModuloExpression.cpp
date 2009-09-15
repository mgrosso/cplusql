#ident "file_id $Id: ModuloExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ModuloExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 20:07:08 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ModuloExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ModuloExpression.h"
#include "BinaryExpressionPriv.h"
#include "Convert.h"

ModuloExpression::ModuloExpression( ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

ModuloExpression::~ModuloExpression()
{
    ;//noop
}

long int
ModuloExpression::getLong() const
{
    return d_->lhs_->getLong() % d_->rhs_->getLong();
}

double              
ModuloExpression::getDouble() const
{
    return static_cast<double>( getLong());
}

long long
ModuloExpression::getLongLong() const
{
    return d_->lhs_->getLongLong() % d_->rhs_->getLongLong();
}

long double
ModuloExpression::getLongDouble() const
{
    return static_cast<long double>( getLongLong());
}

