#ident "file_id $Id: IntegerExpression.cpp,v 1.8 2004/12/31 21:46:53 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IntegerExpression.cpp
// Author:    mgrosso 
// Created:   Mon Apr 28 16:06:23 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: IntegerExpression.cpp,v 1.8 2004/12/31 21:46:53 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "IntegerExpression.h"
#include "ptr_strdup.h"
#include "BaseException.h"

class IntegerExpressionPriv 
{
    private:
    friend class IntegerExpression ;
    IntegerExpressionPriv()
    :v_(0)
    {
        //noop;
    };
    long long int   v_;
    char_ptr        raw_;
};

IntegerExpression::IntegerExpression()
{
    d_=new IntegerExpressionPriv ();
}

IntegerExpression::~IntegerExpression()
{
    delete d_;
}

bool
IntegerExpression::isNull() const
{
    return false;
}

void
IntegerExpression::notify(void)
{
    d_->v_=getLongLong();
    d_->raw_=ptr_strdup_longlong( d_->v_ );
}

const char *
IntegerExpression::getRaw()
{
    return d_->raw_.get();
}

char_ptr
IntegerExpression::getCharPtr() const
{
    return d_->raw_;
}

bool                
IntegerExpression::getBool() const
{
    return d_->v_ ;
}

long int
IntegerExpression::getLong() const
{
    return static_cast< long int > ( d_->v_ );
}

double
IntegerExpression::getDouble() const
{
    return static_cast< double > ( d_->v_ );
}

long double
IntegerExpression::getLongDouble() const
{
    return static_cast< long double > ( d_->v_ );
}

