#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrLeastExpression.cpp
// Author:    mgrosso 
// Created:   Sat Jul 15 18:59:14 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StrLeastExpression.h"
#include <stdio.h>

StrLeastExpression::StrLeastExpression(ExpressionList &args )
:CompareListExpression(args)
{
    ;//noop
}

StrLeastExpression::~StrLeastExpression()
{
    ;//noop
}

ExpressionPtr &
StrLeastExpression::choose(ExpressionPtr &lhs, ExpressionPtr &rhs)
{
    int i= strcmp( lhs->getRaw(), rhs->getRaw() );
    return i < 0 ? lhs : rhs ;
}





