#ident "file_id $Id: WarnExpression.cpp,v 1.1 2005/01/05 01:04:11 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      WarnExpression.cpp
// Author:    mgrosso 
// Created:   Sun Nov 21 18:23:32 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: WarnExpression.cpp,v 1.1 2005/01/05 01:04:11 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "WarnExpression.h"
#include "AppLog.h"
#include "BaseException.h"

WarnExpression::WarnExpression( ExpressionPtr &operand )
:operand_(operand)
{
    ;//noop
}

WarnExpression::~WarnExpression()
{
    ;//noop
}

const char *
WarnExpression::getRaw()
{
    return operand_->getRaw();
}

const char *
WarnExpression::getRaw() const
{
    Expression * e = const_cast<Expression *>(operand_.get());
    return e->getRaw();
}

void
WarnExpression::notify() 
{
    operand_->notify();
    BaseException::dump_stack_trace();
    APPLOG_WARN( operand_->getRaw());
}
