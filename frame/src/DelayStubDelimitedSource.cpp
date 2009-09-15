#ident "file_id $Id: DelayStubDelimitedSource.cpp,v 1.3 2005/01/24 19:31:57 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DelayStubDelimitedSource.cpp
// Author:    mgrosso 
// Created:   Sun Jan  2 21:47:18 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: DelayStubDelimitedSource.cpp,v 1.3 2005/01/24 19:31:57 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "DelayStubDelimitedSource.h"
#include "BaseException.h"
#include "nanosleep_wrap.h"

    

class DelayStubDelimitedSourcePriv 
{
    private:
    int                 line_;
    time_t              seconds_;
    long                nanosecs_ ;
    friend class DelayStubDelimitedSource ;
    DelayStubDelimitedSourcePriv(time_t seconds, long nanosecs)
    :line_(0),seconds_(seconds),nanosecs_(nanosecs)
    {
        ;//noop
    };
};

DelayStubDelimitedSource::DelayStubDelimitedSource(time_t seconds, long nanosecs )
{
    d_=new DelayStubDelimitedSourcePriv (seconds,nanosecs);
}

DelayStubDelimitedSource::~DelayStubDelimitedSource()
{
    delete d_;
}

int
DelayStubDelimitedSource::lineNum()
{
    return d_->line_;
}

bool
DelayStubDelimitedSource::next()
{
    nanosleep_wrap::do_nanosleep(d_->seconds_, d_->nanosecs_ );
    return true;
#if 0
    struct timespec total ;
    struct timespec remaining ;
    int ret;
    total.tv_sec = d_->seconds_;
    total.tv_nsec = d_->nanosecs_;
    while( 
        eintr-- && 
        -1 == (ret=nanosleep( &total, &remaining )) && 
        errno == EINTR 
    ) 
    {
        total.tv_sec = remaining.tv_sec; 
        total.tv_nsec = remaining.tv_nsec;
    }
    if( !eintr )
    {
        PANICV("Too many EINTR, %i in a row is a bit much, eh?", 
            MAX_CONSECUTIVE_EINTR );
    }
    if( ret == -1 )
    {
        PANICV("could not nanosleep()");
    }
    ++d_->line_;
    return true;
#endif 
}

