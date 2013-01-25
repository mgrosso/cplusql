#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrMaxProjection.cpp
// Author:    mgrosso 
// Created:   Mon Jul 17 14:39:23 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "StrMaxProjection.h"


StrMaxProjection::StrMaxProjection(ExpressionPtr &operand)
:StrMinProjection(operand)
{
    ;//noop
}

StrMaxProjection::~StrMaxProjection()
{
    ;//noop
}

int
StrMaxProjection::compare(const char *lhs, const char *rhs)
{
    //if this function returns positive, it means lhs wins.
    return strcmp(rhs,lhs);
}

AbstractProjectionPtr
StrMaxProjection::copy()
{
    AbstractProjectionPtr ap( new StrMaxProjection(getOperand()));
    return ap;
}

