#ident "file_id $Id: NegationExpression.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NegationExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: NegationExpression.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "NegationExpression.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"

NegationExpression::NegationExpression( ExpressionPtr &operand )
:operand_(operand)
{
    ;//noop
}

NegationExpression::~NegationExpression()
{
    ;//noop
}

bool                
NegationExpression::isNull() const
{
    return operand_->isNull();
}

bool                
NegationExpression::getBool() const
{
    return operand_->getBool() ? false : true ;
}

void                
NegationExpression::notify() 
{
    operand_->notify() ;
}

