#ident "file_id $Id: TranslateExpression.cpp,v 1.1 2004/06/02 21:10:57 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      TranslateExpression.cpp
// Author:    mgrosso 
// Created:   Wed May 26 00:54:29 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: TranslateExpression.cpp,v 1.1 2004/06/02 21:10:57 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "TranslateExpression.h"
#include "ByteBuf.h"

class TranslateExpressionPriv 
{
    private:
    friend class TranslateExpression ;
    ExpressionPtr                           op_ ;
    TranslateExpression::translator_t       f_;
    ByteBuf                                 b_;
    TranslateExpressionPriv( ExpressionPtr &operand, TranslateExpression::translator_t f );
};

TranslateExpressionPriv::TranslateExpressionPriv( ExpressionPtr &operand, TranslateExpression::translator_t f )
:op_(operand), f_(f), b_()
{
    ;//noop
};

TranslateExpression::TranslateExpression(ExpressionPtr &operand, translator_t f)
{
    d_=new TranslateExpressionPriv (operand, f );
}

TranslateExpression::~TranslateExpression()
{
    delete d_;
}

void
TranslateExpression::notify()
{
    d_->op_->notify();
    d_->b_.reset();
    const char *s = d_->op_->getRaw();
    char c ;
    while(( c = *s++ ))
    {
        d_->b_.append(static_cast<char>(d_->f_(static_cast<int>(c))));
    }
    d_->b_.append('\0');
}

const char *
TranslateExpression::getRaw() 
{
	return d_->b_.get();
}

const char *
TranslateExpression::getRaw() const
{
	return d_->b_.get();
}

