#ident "file_id $Id: NanosleepExpression.cpp,v 1.1 2005/01/24 19:40:30 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NanosleepExpression.cpp
// Author:    mgrosso 
// Created:   Sat Jan  8 23:43:17 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: NanosleepExpression.cpp,v 1.1 2005/01/24 19:40:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "NanosleepExpression.h"
#include "AppLog.h"
#include "nanosleep_wrap.h"
#include <time.h>

class NanosleepExpressionPriv 
{
    private:
    friend class NanosleepExpression ;

    time_t              seconds_;
    long                nanosecs_ ;

    NanosleepExpressionPriv(time_t seconds, long nanosecs)
    :seconds_(seconds),nanosecs_(nanosecs)
    {
        //noop
    };
};

NanosleepExpression::NanosleepExpression(time_t seconds, long nanosecs)
{
    d_=new NanosleepExpressionPriv (seconds, nanosecs);
}

NanosleepExpression::~NanosleepExpression()
{
    delete d_;
}

const char *
NanosleepExpression::getRaw()
{
    return "1";
}

const char *
NanosleepExpression::getRaw() const
{
    return "1";
}

void
NanosleepExpression::notify()
{
    APPLOG_DBG("nanosleep(%li, %li) ... ", d_->seconds_, d_->nanosecs_ );
    nanosleep_wrap::do_nanosleep(d_->seconds_, d_->nanosecs_ );
    APPLOG_DBG("nanosleep(%li, %li) done", d_->seconds_, d_->nanosecs_ );
}

