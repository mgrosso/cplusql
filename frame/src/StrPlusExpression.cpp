#ident "file_id $Id: StrPlusExpression.cpp,v 1.2 2005/01/03 01:29:30 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrPlusExpression.cpp
// Author:    mgrosso 
// Created:   Fri Oct 29 02:13:26 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: StrPlusExpression.cpp,v 1.2 2005/01/03 01:29:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StrPlusExpression.h"
#include "ByteBuf.h"
#include "AppLog.h"

class StrPlusExpressionPriv 
{
    private:
    friend class StrPlusExpression ;
    ExpressionPtr                           lhs_ ;
    ExpressionPtr                           rhs_ ;
    ByteBuf                                 buf_;

    StrPlusExpressionPriv(
        ExpressionPtr &lhs,  ExpressionPtr &rhs)
    :lhs_(lhs),rhs_(rhs),buf_()
    {
        ;//noop
    };
};

StrPlusExpression::StrPlusExpression( 
    ExpressionPtr &lhs,  ExpressionPtr &rhs)
{
    d_=new StrPlusExpressionPriv (lhs, rhs );
}

StrPlusExpression::~StrPlusExpression()
{
    delete d_;
}

void
StrPlusExpression::notify()
{
    //APPLOG_DBG( ".");
    d_->lhs_->notify();
    d_->rhs_->notify();
    d_->buf_.reset();
    //APPLOG_DBG( "buf_=%s", d_->buf_.get());
    d_->buf_.appendNoNull( d_->lhs_->getRaw());
    //APPLOG_DBG( "buf_=%s", d_->buf_.get());
    d_->buf_.append( d_->rhs_->getRaw());
    //APPLOG_DBG( "buf_=%s", d_->buf_.get());
}

const char *
StrPlusExpression::getRaw() 
{
	return d_->buf_.get();
}

const char *
StrPlusExpression::getRaw() const
{
	return d_->buf_.get();
}



