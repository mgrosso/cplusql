#ident "file_id $Id: LongLongFormatExpression.cpp,v 1.1 2005/01/04 06:45:34 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LongLongFormatExpression.cpp
// Author:    mgrosso 
// Created:   Mon Jan  3 19:30:36 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: LongLongFormatExpression.cpp,v 1.1 2005/01/04 06:45:34 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdio.h>
#include "LongLongFormatExpression.h"

class LongLongFormatExpressionPriv 
{
    private:
    char_ptr        format_;
    ExpressionPtr   operand_ ;
    long long       v_ ;
    char            raw_[64];
    friend class LongLongFormatExpression ;
    LongLongFormatExpressionPriv(
        const char *format, ExpressionPtr &operand )
    :format_(ptr_strdup(format)), operand_(operand)
    {
        ;//noop
    };
};

LongLongFormatExpression::LongLongFormatExpression( 
    const char *format, ExpressionPtr &operand )
{
    d_=new LongLongFormatExpressionPriv (format,operand);
}
LongLongFormatExpression::LongLongFormatExpression(ExpressionPtr &operand)
{
    d_=new LongLongFormatExpressionPriv (DFLT_LLFORMAT,operand);
}

LongLongFormatExpression::~LongLongFormatExpression()
{
    delete d_;
}

void
LongLongFormatExpression::notify()
{
    d_->operand_->notify();
    d_->v_ = static_cast<long long>(d_->operand_->getLongDouble());
    snprintf( d_->raw_, 63, d_->format_.get(), d_->v_ );
    d_->raw_[63]='\0';
}

const char *
LongLongFormatExpression::getRaw()
{
    return d_->raw_;
}

const char *
LongLongFormatExpression::getRaw() const
{
    return d_->raw_;
}

long double
LongLongFormatExpression::getLongDouble() const
{
    return d_->v_;
}

long long
LongLongFormatExpression::getLongLong() const
{
    return d_->v_;
}

double
LongLongFormatExpression::getDouble() const
{
    return d_->v_;
}

long
LongLongFormatExpression::getLong() const
{
    return d_->v_;
}

