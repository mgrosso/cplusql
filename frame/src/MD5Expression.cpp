#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MD5Expression.cpp
// Author:    mgrosso 
// Created:   Thu Jun 29 04:10:01 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MD5Expression.h"
#include "ByteBuf.h"
#include "md5.h"

class MD5ExpressionPriv 
{
    private:
    friend class MD5Expression ;
    ExpressionPtr   operand_ ;
    ByteBuf         buf_ ;
    MD5             md5_;
    MD5ExpressionPriv(ExpressionPtr &operand )
    :operand_(operand)
    {
        ;//
    };
};

MD5Expression::MD5Expression( ExpressionPtr &operand )
{
    d_=new MD5ExpressionPriv (operand);
}

MD5Expression::~MD5Expression()
{
    delete d_;
}

const char *
MD5Expression::getRaw()
{
    return d_->buf_.get();
}

const char *
MD5Expression::getRaw() const
{
    return d_->buf_.get();
}

void
MD5Expression::notify()
{
    d_->operand_->notify();
    d_->md5_.init();
    const char *raw = d_->operand_->getRaw();
    unsigned int len = strlen(raw);
    d_->md5_.update((unsigned char *)raw,len);
    d_->md5_.finalize();
    d_->buf_.reset( d_->md5_.hex_digest());
}

