#ident "file_id $Id: LnExpression.cpp,v 1.2 2004/09/22 21:07:21 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LnExpression.cpp
// Author:    mgrosso 
// Created:   Sat Sep  4 04:36:16 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: LnExpression.cpp,v 1.2 2004/09/22 21:07:21 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <math.h>
#include <errno.h>
#include "frame_config.h"
#include "LnExpression.h"


LnExpression::LnExpression( ExpressionPtr &operand, ExpressionPtr &ifedom )
:RangedUnaryArithmeticExpression(operand, ifedom)
{
    ;//noop
}

LnExpression::~LnExpression()
{
    ;//noop
}

long double
LnExpression::getLongDouble(long double op) 
{
    errno=0;
    long double reply ;
#ifdef HAVE_LOGL
    reply = logl(op);
#else
    reply = static_cast<long double>(log(static_cast<double>(op)));
#endif
    if( errno && ( errno == EDOM || errno == ERANGE ))
    {
        return edom();
    }
    return reply;
}
