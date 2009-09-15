#ident "file_id $Id: UnaryExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      UnaryExpression.cpp
// Author:    mgrosso 
// Created:   Tue Jun  3 23:03:10 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: UnaryExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "UnaryExpression.h"

    
UnaryExpression::UnaryExpression( ExpressionPtr &e )
:operand_(e)
{
    ;//noop
}

UnaryExpression::~UnaryExpression()
{
    ;//noop
}

bool
UnaryExpression::isNull() const
{
	return operand_->isNull();
}

void
UnaryExpression::notify()
{
    operand_->notify();
}
