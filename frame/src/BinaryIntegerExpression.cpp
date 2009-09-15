#ident "file_id $Id: BinaryIntegerExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BinaryIntegerExpression.cpp
// Author:    mgrosso 
// Created:   Fri Oct 29 00:36:09 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: BinaryIntegerExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BinaryIntegerExpression.h"
#include "BinaryExpressionPriv.h"

BinaryIntegerExpression::BinaryIntegerExpression( 
    ExpressionPtr &lhs, ExpressionPtr &rhs )
{
    d_=new BinaryExpressionPriv (lhs, rhs);
}

BinaryIntegerExpression::~BinaryIntegerExpression()
{
    delete d_;
}

void
BinaryIntegerExpression::notify(void)
{
    d_->lhs_->notify();
    d_->rhs_->notify();
    IntegerExpression::notify();
}

