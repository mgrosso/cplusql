#ident "file_id $Id: LogicalAndExpression.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogicalAndExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LogicalAndExpression.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "LogicalAndExpression.h"
#include "BinaryExpressionPriv.h"
#include <stdio.h>

LogicalAndExpression::LogicalAndExpression( 
    ExpressionPtr lhs, ExpressionPtr rhs )
:LogicalExpression( lhs, rhs ), value_(false)
{
    ;//noop
}

LogicalAndExpression::~LogicalAndExpression()
{
    ;//noop
}

bool
LogicalAndExpression::getBool() const
{
    return value_;
}

void
LogicalAndExpression::notify( void ) 
{
    d_->lhs_->notify() ;
    if( ! d_->lhs_->getBool() )
    {
        //avoid evaluating rhs if possible
        value_=false;
        return;
    }
    d_->rhs_->notify();
    value_ = d_->rhs_->getBool();
}

