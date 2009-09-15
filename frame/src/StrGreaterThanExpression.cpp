#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrGreaterThanExpression.cpp
// Author:    mgrosso 
// Created:   Fri Jul 14 17:56:34 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StrGreaterThanExpression.h"
#include "BinaryExpressionPriv.h"
#include <stdio.h>


StrGreaterThanExpression::StrGreaterThanExpression( 
    ExpressionPtr lhs, ExpressionPtr rhs )
:LogicalExpression( lhs, rhs )
{
}

StrGreaterThanExpression::~StrGreaterThanExpression()
{
}

bool                
StrGreaterThanExpression::getBool() const
{
    int i= strcmp( d_->lhs_->getRaw() ,  d_->rhs_->getRaw() );
    //FDBG_SSI( d_->lhs_->getRaw(),  d_->rhs_->getRaw(), i );
    return i > 0 ? true : false;
}

