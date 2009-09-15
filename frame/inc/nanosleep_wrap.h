// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      nanosleep_wrap.h
// Author:    mgrosso 
// Created:   Sat Jan  8 23:57:08 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: nanosleep_wrap.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef NANOSLEEP_WRAP_H
#define NANOSLEEP_WRAP_H 1

#include "frame_config.h"
    
class nanosleep_wrap
{
    public:
    static void do_nanosleep( time_t secs, long nanoseconds );

    private:
    //not implemented
    nanosleep_wrap();
    nanosleep_wrap( const nanosleep_wrap &rhs );
    nanosleep_wrap &operator=( const nanosleep_wrap &rhs );
};

#endif /* NANOSLEEP_WRAP_H */
