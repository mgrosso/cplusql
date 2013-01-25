#ident "file_id $Id: SQLEscapeExpression.cpp,v 1.2 2005/10/19 08:41:19 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SQLEscapeExpression.cpp
// Author:    mgrosso 
// Created:   Sat Jan  8 16:28:42 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SQLEscapeExpression.cpp,v 1.2 2005/10/19 08:41:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "SQLEscapeExpression.h"
#include "ByteBuf.h"
#include "BaseException.h"
#if FRAME_USES_POSTGRES
#include <libpq-fe.h>
#endif


class SQLEscapeExpressionPriv 
{
    private:
    ByteBuf         buf_;
    ExpressionPtr   operand_;

    friend class SQLEscapeExpression ;
    SQLEscapeExpressionPriv( ExpressionPtr &operand )
    :buf_(512, 8192 ), operand_(operand)
    {
        //noop
    };
};

SQLEscapeExpression::SQLEscapeExpression(ExpressionPtr &operand )
{
    d_=new SQLEscapeExpressionPriv (operand);
}

SQLEscapeExpression::~SQLEscapeExpression()
{
    delete d_;
}

const char *
SQLEscapeExpression::getRaw()
{
    return d_->buf_.get();
}

const char *
SQLEscapeExpression::getRaw() const
{
    return d_->buf_.get();
}

void
SQLEscapeExpression::notify()
{
    d_->operand_->notify();
    const char *src = d_->operand_->getRaw();
    size_t sz = strlen(src);
    char *dest = d_->buf_.reset( sz * 2 );
    //TODO  this needs to be at least ecumenical if not atheist, where religion
    //is the databases supported.
#if FRAME_USES_POSTGRES
    PQescapeString( dest, src, sz );
#else
    PANICV("support for postgres not compiled in. TODO: implement PQescapeString in cross database way.");
    strcpy(dest,src);//strncpy not needed. plenty of room.
#endif
}

