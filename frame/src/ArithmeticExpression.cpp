#ident "file_id $Id: ArithmeticExpression.cpp,v 1.8 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ArithmeticExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ArithmeticExpression.cpp,v 1.8 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "ArithmeticExpression.h"
#include "BinaryExpressionPriv.h"
#include "BaseException.h"
#include "AppLog.h"

ArithmeticExpression::ArithmeticExpression( ExpressionPtr &lhs, ExpressionPtr &rhs )
{
    d_=new BinaryExpressionPriv(lhs, rhs );
}

ArithmeticExpression::~ArithmeticExpression()
{
    delete d_;
}

bool                
ArithmeticExpression::isNull() const
{
    if( d_->lhs_->isNull() || d_->rhs_->isNull())
    {
        return true;
    }
    return false;
}

const char *
ArithmeticExpression::getRaw()
{
    return d_->raw_.get();
}

char_ptr
ArithmeticExpression::getCharPtr() const
{
    return d_->raw_;
}

bool                
ArithmeticExpression::getBool() const
{
    return getLong() ? true : false ;
}

void
ArithmeticExpression::notify()
{
    d_->lhs_->notify();
    d_->rhs_->notify();
    childNotify();
    d_->raw_=ptr_strdup_longdouble( getLongDouble());
}

void
ArithmeticExpression::childNotify()
{
    ;//noop, hook can be overridden by children
}

