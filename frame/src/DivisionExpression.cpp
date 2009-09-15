#ident "file_id $Id: DivisionExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DivisionExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 20:07:08 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DivisionExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "DivisionExpression.h"
#include "BinaryExpressionPriv.h"
#include "Convert.h"
#include "BaseException.h"

DivisionExpression::DivisionExpression( ExpressionPtr &lhs, ExpressionPtr &rhs )
:ArithmeticExpression(lhs,rhs)
{
    ;//noop
}

DivisionExpression::~DivisionExpression()
{
    ;//noop
}

long int
DivisionExpression::getLong() const
{
    long int right = d_->rhs_->getLong();
    if( right == 0 )
    {
        PANIC( "DivisionExpression::getLong(), divide by zero");
    }
    return d_->lhs_->getLong() / right;
}

double              
DivisionExpression::getDouble() const
{
    double right = d_->rhs_->getDouble();
    if( right == 0 )
    {
        PANIC( "DivisionExpression::getDouble(), divide by zero");
    }
    return d_->lhs_->getDouble() / right ;
}

long long
DivisionExpression::getLongLong() const
{
    long long right = d_->rhs_->getLongLong();
    if( right == 0 )
    {
        PANIC( "DivisionExpression::getLongLong(), divide by zero");
    }
    return d_->lhs_->getLongLong() / right ;
}

long double
DivisionExpression::getLongDouble() const
{
    long double right = d_->rhs_->getLongDouble();
    if( right == 0 )
    {
        PANIC( "DivisionExpression::getLongDouble(), divide by zero");
    }
    return d_->lhs_->getLongDouble() / right ;
}

