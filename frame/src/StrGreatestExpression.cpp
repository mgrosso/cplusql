#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrGreatestExpression.cpp
// Author:    mgrosso 
// Created:   Sat Jul 15 18:38:37 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StrGreatestExpression.h"
#include <string.h>

StrGreatestExpression::StrGreatestExpression(ExpressionList &args )
:CompareListExpression(args)
{
    ;//noop
}

StrGreatestExpression::~StrGreatestExpression()
{
    ;//noop
}

ExpressionPtr &
StrGreatestExpression::choose(ExpressionPtr &lhs, ExpressionPtr &rhs)
{
    int i= strcmp( lhs->getRaw(), rhs->getRaw() );
    return i > 0 ? lhs : rhs ;
}

