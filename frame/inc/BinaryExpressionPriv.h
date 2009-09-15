// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BinaryExpressionPriv.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Nov 22 20:15:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: BinaryExpressionPriv.h,v 1.2 2003/06/10 01:54:40 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef BINARYEXPRESSIONPRIV_H
#define BINARYEXPRESSIONPRIV_H 1

#include "ptr.h"
#include "ptr_strdup.h"
#include "Expression.h"

class BinaryExpressionPriv
{
public:
    ExpressionPtr lhs_;
    ExpressionPtr rhs_;
    char_ptr      raw_;
    BinaryExpressionPriv( ExpressionPtr &lhs, ExpressionPtr &rhs )
    :lhs_(lhs),rhs_(rhs)
    {
        ;//noop
    }
};


#endif /* BINARYEXPRESSIONPRIV_H */



