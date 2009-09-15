#ident "file_id $Id: StrLenExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrLenExpression.cpp
// Author:    mgrosso 
// Created:   Fri Oct 29 01:48:04 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: StrLenExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StrLenExpression.h"
#include "BinaryExpressionPriv.h"
#include "string.h"

    

StrLenExpression::StrLenExpression( ExpressionPtr operand )
:BinaryIntegerExpression( operand, operand )
{
}

StrLenExpression::~StrLenExpression()
{
}

long long
StrLenExpression::getLongLong() const
{
    return strlen( d_->lhs_->getRaw());
}


void
StrLenExpression::notify(void)
{
        d_->lhs_->notify();
        //d_->rhs_->notify();
        IntegerExpression::notify();
}




