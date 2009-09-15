#ident "file_id $Id: nanosleep_wrap.cpp,v 1.1 2005/01/24 19:40:31 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      nanosleep_wrap.cpp
// Author:    mgrosso 
// Created:   Sat Jan  8 23:57:08 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: nanosleep_wrap.cpp,v 1.1 2005/01/24 19:40:31 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "nanosleep_wrap.h"
#include "BaseException.h"

void
nanosleep_wrap::do_nanosleep(time_t secs, long nanoseconds )
{
    int eintr = MAX_CONSECUTIVE_EINTR ;
    struct timespec total ;
    struct timespec remaining ;
    int ret;
    total.tv_sec = secs ;
    total.tv_nsec = nanoseconds ;
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
}

