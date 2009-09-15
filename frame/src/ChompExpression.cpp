#ident "file_id $Id: ChompExpression.cpp,v 1.2 2005/01/05 01:54:01 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ChompExpression.cpp
// Author:    mgrosso 
// Created:   Sun Jan  2 16:13:28 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: ChompExpression.cpp,v 1.2 2005/01/05 01:54:01 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ChompExpression.h"
#include "ByteBuf.h"

class ChompExpressionPriv 
{
    private:
    friend class ChompExpression ;
    ExpressionPtr   operand_ ;
    ByteBuf         buf_ ;
    ChompExpressionPriv(ExpressionPtr &operand )
    :operand_(operand)
    {
        ;//
    };
};

ChompExpression::ChompExpression( ExpressionPtr &operand )
{
    d_=new ChompExpressionPriv (operand);
}

ChompExpression::~ChompExpression()
{
    delete d_;
}

const char *
ChompExpression::getRaw()
{
    return d_->buf_.get();
}

const char *
ChompExpression::getRaw() const
{
    return d_->buf_.get();
}

void
ChompExpression::notify()
{
    d_->operand_->notify();
    d_->buf_.reset( d_->operand_->getRaw());
    char *s = d_->buf_.buf();
    char *e = s;
    while( *e ){ ++e; };
    while( --e  >= s  && ( *e == '\n' || *e == '\r' ))
    {
        *e = '\0';
    }
}

