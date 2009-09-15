#ident "file_id $Id: SubStrExpression.cpp,v 1.2 2005/01/26 20:38:21 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SubStrExpression.cpp
// Author:    mgrosso 
// Created:   Wed Jan 12 17:00:01 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SubStrExpression.cpp,v 1.2 2005/01/26 20:38:21 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SubStrExpression.h"
#include <string.h>
#include "ByteBuf.h"
#include "AppLog.h"

class SubStrExpressionPriv 
{
    friend class SubStrExpression ;
    private:
    ExpressionPtr           operand_; 
    ExpressionPtr           start_;
    ExpressionPtr           length_;
    ByteBuf                 buf_;
    SubStrExpressionPriv(
        ExpressionPtr &operand,
        ExpressionPtr &start,
        ExpressionPtr &length
    ):operand_(operand), start_(start), length_(length)
    {
        //noop
    };
    void notify();
};

SubStrExpression::SubStrExpression( 
    ExpressionPtr &operand,
    ExpressionPtr &start,
    ExpressionPtr &length
    )
{
    //APPLOG_DBG("ehlo?");
    d_ = new SubStrExpressionPriv( operand, start, length) ;
}

SubStrExpression::~SubStrExpression()
{
    delete d_;
}

void
SubStrExpressionPriv::notify()
{
    operand_->notify();
    start_->notify();
    length_->notify();
    const char *s = operand_->getRaw();
    size_t begin =  start_->getLong();
    size_t len =    length_->getLong();
    buf_.reset( len+1 );
    size_t slen = strlen(s);
    if( slen < begin )
    {
        //APPLOG_DBG("slen=%lu begin=%lu len=%lu op=(%s) op+begin=(%s)",
        //    slen, begin, len, s, s+begin );
        return;
    }
    char *dest=buf_.buf();
    strncpy( dest, s+begin, len );
    *(dest + len)='\0';
    //APPLOG_DBG("slen=%lu begin=%lu len=%lu op=(%s) dest=(%s)",
    //    slen, begin, len, s, s+begin, dest );
}

void
SubStrExpression::notify()
{
    d_->notify();
}

const char *
SubStrExpression::getRaw()
{
    return d_->buf_.get();
}

const char *
SubStrExpression::getRaw() const
{
    return d_->buf_.get();
}

