#ident "file_id $Id: Log2Expression.cpp,v 1.1 2005/02/01 01:24:29 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Log2Expression.cpp
// Author:    mgrosso 
// Created:   Fri Jan 28 14:40:36 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: Log2Expression.cpp,v 1.1 2005/02/01 01:24:29 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <math.h>
#include <errno.h>
#include "frame_config.h"
#include "Log2Expression.h"

Log2Expression::Log2Expression(
    ExpressionPtr &operand, ExpressionPtr &ifedom )
:RangedUnaryArithmeticExpression(operand, ifedom)
{
    ;//noop
}

Log2Expression::~Log2Expression()
{
    ;//noop
}

long double
Log2Expression::getLongDouble(long double op) 
{
    errno=0;
    long double reply ;
#ifdef HAVE_LOGL
    reply = log2l(op);
#else
    reply = static_cast<long double>(log2(static_cast<double>(op)));
#endif
    if( errno && ( errno == EDOM || errno == ERANGE ))
    {
        return edom();
    }
    return reply;
}

