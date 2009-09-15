#ident "file_id $Id: GreaterThanExpression.cpp,v 1.1 2004/05/14 14:34:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      GreaterThanExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: GreaterThanExpression.cpp,v 1.1 2004/05/14 14:34:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "GreaterThanExpression.h"
#include "BinaryExpressionPriv.h"

GreaterThanExpression::GreaterThanExpression( 
    ExpressionPtr lhs, ExpressionPtr rhs )
:LogicalExpression( lhs, rhs )
{
}

GreaterThanExpression::~GreaterThanExpression()
{
}

bool                
GreaterThanExpression::getBool() const
{
    return ( d_->lhs_->getLongDouble() > d_->rhs_->getLongDouble() );
}

