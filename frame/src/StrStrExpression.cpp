#ident "file_id $Id: StrStrExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrStrExpression.cpp
// Author:    mgrosso 
// Created:   Sun Oct 24 22:15:38 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: StrStrExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StrStrExpression.h"
#include "BinaryExpressionPriv.h"
#include "string.h"


StrStrExpression::StrStrExpression( ExpressionPtr lhs, ExpressionPtr rhs )
:BinaryIntegerExpression( lhs, rhs )
{
    ;//noop
}

StrStrExpression::~StrStrExpression()
{
    ;//noop
}

long long
StrStrExpression::getLongLong() const
{
    const char *haystack =  d_->lhs_->getRaw() ;
    const char *s = strstr( haystack, d_->rhs_->getRaw());
    return s ?  s - haystack : 0 ;
}

