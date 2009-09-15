
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      twrap.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 25 04:35:25 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: twrap.h,v 1.1 2004/08/07 15:49:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef TWRAP_H
#define TWRAP_H 1

#define NEED_GETTIMEOFDAY
#include "frame_config.h"
#include "ptr_strdup.h"

class twrap
{
public:
    static void gettimeofday( timeval *tv );
    static void ctime_r( time_t *t, char *buf, unsigned int bufsz );
    static void getctime( char *buf, unsigned int bufsz, suseconds_t &usec );
    static char_ptr getctime_ptr( time_t *t );
};

#endif /* TWRAP_H */
