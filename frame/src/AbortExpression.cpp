#ident "file_id $Id: AbortExpression.cpp,v 1.7 2004/06/21 06:11:47 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbortExpression.cpp
// Author:    mgrosso 
// Created:   Thu Apr  3 06:23:41 EST 2003 on caliban.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: AbortExpression.cpp,v 1.7 2004/06/21 06:11:47 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AbortExpression.h"
#include "ptr_strdup.h"
#include "BaseException.h"
#include "Convert.h"

    

AbortExpression::AbortExpression( ExpressionPtr &e )
:UnaryExpression(e)
{
    ;//noop
}

AbortExpression::~AbortExpression()
{
    ;//noop
}

void
AbortExpression::doThrow() const
{
    char_ptr c = operand_->getCharPtr();
    PANICV( "AbortExpression [%s]", c.get());
}

bool                
AbortExpression::isNull() const
{
    doThrow();
    //not reached.
    return true;
}

const char *
AbortExpression::getRaw()
{
    doThrow();
    //not reached.
    return NULL;
}

char_ptr
AbortExpression::getCharPtr() const
{
    doThrow();
    //not reached.
    return operand_->getCharPtr();
}

bool                
AbortExpression::getBool() const
{
    doThrow();
    //not reached.
    return false;
}

long int
AbortExpression::getLong() const
{
    doThrow();
    //not reached.
    return 0;
}

double              
AbortExpression::getDouble() const
{
    doThrow();
    //not reached.
    return 0;
}

long long
AbortExpression::getLongLong() const
{
    doThrow();
    //not reached.
    return 0;
}

long double
AbortExpression::getLongDouble() const
{
    doThrow();
    //not reached.
    return 0;
}


