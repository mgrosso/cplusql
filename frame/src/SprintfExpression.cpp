#ident "file_id $Id: SprintfExpression.cpp,v 1.1 2005/01/24 19:46:31 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SprintfExpression.cpp
// Author:    mgrosso 
// Created:   Sat Jan  8 02:15:09 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SprintfExpression.cpp,v 1.1 2005/01/24 19:46:31 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "SprintfExpression.h"
#include "BaseException.h"
#include "AppLog.h"
#include "ByteBuf.h"
#include <stdio.h>
#include <string.h>

    

class SprintfExpressionPriv 
{
    private:
    char_ptr        format_ ;
    ExpressionList  values_;
    ByteBuf         buf_;
    ptr<char *>     pieces_;
    friend class SprintfExpression ;
    SprintfExpressionPriv( const char *format, ExpressionList values )
    :format_(ptr_strdup(format)), values_(values), buf_( 1024, 8192 ),
        pieces_( new char *[ values_->size()+1 ])
    {
        size_t sz = values_->size();
        char *f = format_.get();
        char *e = f + strlen(format_.get());
        size_t i;
        size_t found = 0;
        for( i=0; i < sz + 1 && f && f<e && *f ; ++i )
        {
            if( i < sz+1 )
            {
                pieces_[i]=f;
            }
            f = strstr( f, "%s" );
            if( f )
            {
                ++found;
                *f='\0';
                f=f+2;
                if(! *f && i+1 < sz+1 )
                {
                    pieces_[i+1]= "" ;
                }
            }
        }
        if( found != sz )
        {
            PANICV( "%lu not enough or too many %%s for %lu values in [%s]", 
                found, sz, format_.get());
        }
    };
};

SprintfExpression::SprintfExpression( 
    const char *format, ExpressionList values )
{
    d_=new SprintfExpressionPriv (format, values );
}

SprintfExpression::~SprintfExpression()
{
    delete d_;
}

const char *
SprintfExpression::getRaw()
{
    return d_->buf_.get();
}

const char *
SprintfExpression::getRaw() const
{
    return d_->buf_.get();
}

void
SprintfExpression::notify()
{
    size_t j;
    ExpressionListItr i,e;
    d_->buf_.reset();
    for( i=d_->values_->begin(), e=d_->values_->end(), j=0; i!=e; ++i, ++j)
    {
        (*i)->notify();
        d_->buf_.appendNoNull(d_->pieces_[j]);
        d_->buf_.appendNoNull((*i)->getRaw());
    }
    d_->buf_.append(d_->pieces_[j]);
}

