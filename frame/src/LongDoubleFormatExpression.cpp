#ident "file_id $Id: LongDoubleFormatExpression.cpp,v 1.1 2004/09/21 16:09:08 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LongDoubleFormatExpression.cpp
// Author:    mgrosso 
// Created:   Mon Sep  6 19:41:56 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: LongDoubleFormatExpression.cpp,v 1.1 2004/09/21 16:09:08 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "LongDoubleFormatExpression.h"
#include "RangedUnaryArithmeticExpressionPriv.h"

class LongDoubleFormatExpressionPriv 
{
    private:
    char_ptr format_;
    friend class LongDoubleFormatExpression ;
    LongDoubleFormatExpressionPriv( const char *format )
    :format_(ptr_strdup(format))
    {
        ;//noop
    };
};

LongDoubleFormatExpression::LongDoubleFormatExpression( const char *format, ExpressionPtr &operand )
:RangedUnaryArithmeticExpression( operand,  operand )
    //second arg to parent class ctor will never be used, we just need a valid ref
{
    ld_=new LongDoubleFormatExpressionPriv (format);
}

LongDoubleFormatExpression::~LongDoubleFormatExpression()
{
    delete ld_;
}

const char *
LongDoubleFormatExpression::getRaw()
{
    snprintf( d_->raw_, 63, ld_->format_.get(), d_->value_ );
    d_->raw_[63]='\0';
    return d_->raw_;
}

long double
LongDoubleFormatExpression::getLongDouble( long double op )
{
    return op;
}
