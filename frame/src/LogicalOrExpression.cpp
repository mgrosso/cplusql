#ident "file_id $Id: LogicalOrExpression.cpp,v 1.8 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogicalOrExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 22:11:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LogicalOrExpression.cpp,v 1.8 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "LogicalOrExpression.h"
#include "BinaryExpressionPriv.h"

LogicalOrExpression::LogicalOrExpression( 
    ExpressionPtr lhs, ExpressionPtr rhs )
:LogicalExpression( lhs, rhs )
{
    ;//noop
}

LogicalOrExpression::~LogicalOrExpression()
{
    ;//noop
}

bool                
LogicalOrExpression::getBool() const
{
    return value_;
}

void
LogicalOrExpression::notify( void ) 
{
    d_->lhs_->notify() ;
    if( d_->lhs_->getBool() )
    {
        //avoid evaluating rhs if possible
        value_=true;
        return;
    }
    d_->rhs_->notify();
    value_ = d_->rhs_->getBool();
}


