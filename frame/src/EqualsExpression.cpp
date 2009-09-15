#ident "file_id $Id: EqualsExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      EqualsExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 23:21:16 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: EqualsExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "EqualsExpression.h"
#include "BinaryExpressionPriv.h"

EqualsExpression::EqualsExpression( 
    ExpressionPtr lhs, ExpressionPtr rhs )
:LogicalExpression( lhs, rhs )
{
}

EqualsExpression::~EqualsExpression()
{
}

bool                
EqualsExpression::getBool() const
{
    return d_->lhs_->getLongDouble() == d_->rhs_->getLongDouble();
}






