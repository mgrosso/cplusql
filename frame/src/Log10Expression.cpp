#ident "file_id $Id: Log10Expression.cpp,v 1.1 2005/02/01 01:24:29 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Log10Expression.cpp
// Author:    mgrosso 
// Created:   Fri Jan 28 14:21:23 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: Log10Expression.cpp,v 1.1 2005/02/01 01:24:29 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <math.h>
#include <errno.h>
#include "frame_config.h"
#include "Log10Expression.h"

Log10Expression::Log10Expression( 
    ExpressionPtr &operand, ExpressionPtr &ifedom )
:RangedUnaryArithmeticExpression(operand, ifedom)
{
    ;//noop
}

Log10Expression::~Log10Expression()
{
    ;//noop
}

long double
Log10Expression::getLongDouble(long double op) 
{
    errno=0;
    long double reply ;
#ifdef HAVE_LOGL
    reply = log10l(op);
#else
    reply = static_cast<long double>(log10(static_cast<double>(op)));
#endif
    if( errno && ( errno == EDOM || errno == ERANGE ))
    {
        return edom();
    }
    return reply;
}

