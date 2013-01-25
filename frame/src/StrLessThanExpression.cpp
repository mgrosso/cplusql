#ident "file_id $Id: StrLessThanExpression.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrLessThanExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: StrLessThanExpression.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include <string.h>
#include "StrLessThanExpression.h"
#include "BinaryExpressionPriv.h"
#include "dbg.h"

StrLessThanExpression::StrLessThanExpression( 
    ExpressionPtr lhs, ExpressionPtr rhs )
:LogicalExpression( lhs, rhs )
{
}

StrLessThanExpression::~StrLessThanExpression()
{
}

bool                
StrLessThanExpression::getBool() const
{
    int i= strcmp( d_->lhs_->getRaw() ,  d_->rhs_->getRaw() );
    //FDBG_SSI( d_->lhs_->getRaw(),  d_->rhs_->getRaw(), i );
    return i < 0 ? true : false;
}


