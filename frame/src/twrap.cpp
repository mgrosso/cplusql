#ident "file_id $Id: twrap.cpp,v 1.2 2004/08/24 21:56:39 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      twrap.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 25 04:41:38 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: twrap.cpp,v 1.2 2004/08/24 21:56:39 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "twrap.h"
#include <sys/time.h>
#include <string.h>

void
twrap::ctime_r( time_t *t, char *buf, unsigned int bufsz )
{
#ifdef HAVE_CTIME_R
    if(bufsz < CTIME_MIN_BUF_SIZE )
    {
        *buf='\0';
        return;
    }
    ::ctime_r( t, buf );
#else
#ifdef FRAME_USES_ACE
    (void)ACE_OS::ctime_r( t, buf, bufsz );
#else /* FRAME_USES_ACE */
    ;//noop
#endif /* FRAME_USES_ACE */
#endif /*  HAVE_CTIME_R */
    char *newline=strchr( buf, '\n' );
    if( newline )
    {
        *newline = '\0';
    }
}

void
twrap::gettimeofday(timeval *tv )
{
#ifndef FRAME_MISSING_GETTIMEOFDAY
//gettimeofday(struct timeval *tv, struct timezone *tz);
           
    (void) ::gettimeofday( tv, 0 );
#else
    tv->tv_sec=time(NULL);
    tv->tv_usec=0;
#endif
}
void
twrap::getctime( char *buf, unsigned int bufsz, suseconds_t &usec )
{
    struct timeval tv;
    twrap::gettimeofday( &tv );
    twrap::ctime_r( &tv.tv_sec, buf, bufsz );
    usec = tv.tv_usec ;
}

char_ptr
twrap::getctime_ptr( time_t *t )
{
    char_ptr giveback = new char[CTIME_MIN_BUF_SIZE+1]; 
    twrap::ctime_r( t, giveback.get(), CTIME_MIN_BUF_SIZE +1);
    return giveback;
}


