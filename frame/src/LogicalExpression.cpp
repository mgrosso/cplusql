#ident "file_id $Id: LogicalExpression.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogicalExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LogicalExpression.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "LogicalExpression.h"
#include "BinaryExpressionPriv.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"

LogicalExpression::LogicalExpression( ExpressionPtr lhs, ExpressionPtr rhs )
{
    d_=new BinaryExpressionPriv(lhs,rhs);//
}

LogicalExpression::~LogicalExpression()
{
    delete d_;
}

bool                
LogicalExpression::isNull() const
{
    if( d_->lhs_->isNull() || d_->rhs_->isNull())
    {
        return true;
    }
    return false;
}

void
LogicalExpression::notify() 
{
    d_->lhs_->notify();
    d_->rhs_->notify();
}
