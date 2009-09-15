#ident "file_id $Id: LessThanExpression.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LessThanExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LessThanExpression.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "LessThanExpression.h"
#include "BinaryExpressionPriv.h"

LessThanExpression::LessThanExpression( 
    ExpressionPtr lhs, ExpressionPtr rhs )
:LogicalExpression( lhs, rhs )
{
}

LessThanExpression::~LessThanExpression()
{
}

bool                
LessThanExpression::getBool() const
{
    return ( d_->lhs_->getLongDouble() < d_->rhs_->getLongDouble() );
}


