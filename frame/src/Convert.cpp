#ident "file_id $Id: Convert.cpp,v 1.10 2005/04/25 17:43:56 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Convert.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 15:35:37 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: Convert.cpp,v 1.10 2005/04/25 17:43:56 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_LIMITS_H
#include "frame_config.h"

#include <string.h>
#include "Convert.h"
#include "BaseException.h"
#include <stdlib.h>
#include <float.h>
#include <cstdlib>
#include <limits.h>

void 
checkNull( const char *s )
{
    if( Convert::isNull( s ))
    {
        PANICV( "Convert::*(): cannot convert null string" );
    }
};

bool                 
Convert::isNull( const char *s )
{
    if( !s || !*s )
    {
        return true ;
    }
    return false;
}

bool                 
Convert::convertToBool( const char *s )
{
    checkNull(s);
    //yes, no, true, false, all work
    if( *s == '1' || *s == 't' || *s == 'y' || *s == 'T' || *s == 'Y' )
    {
        return true;
    }
    if( (*s == '0' && *(s+1)=='\0') || *s == 'f' || *s == 'n' || *s == 'F' || *s == 'N' )
    {
        return false;
    }

    if(!strcasecmp( "enable", s ))
        return true;
    if(!strcasecmp( "disable", s ))
        return false;
    int intval=convertToLong(s);
    if(intval==0)
        return false ;
    return true;
}

long int                  
Convert::convertToLong( const char *s )
{
    checkNull(s);
    char *e;
    long int i=strtol( s, &e, 0);
    if( errno == ERANGE || i==LONG_MIN || i==LONG_MAX || (e && *e && (e==s || e==(s+1))))
    {
        PANICV( "strtol failed on [%s]", s );
    }
    return i;
}

int                  
Convert::convertToInt( const char *s )
{
    return static_cast<int>(convertToLong(s));
}

double               
Convert::convertToDouble( const char *s )
{
    checkNull(s);
    char *e;
    int olderr=errno;
    errno=0;
    double d=strtod( s, &e);
    if( errno == ERANGE 
        || d==DBL_MAX || d==DBL_MIN
        || ( e && *e && ( e==s || e==(s+1))))
    {
        PANICV( "strtod failed on [%s]", s );
    }
    errno=olderr;
    return d;
}

long long            
Convert::convertToLongLong( const char *s )
{
    checkNull(s);
    char *e;
    int olderr=errno;
    errno=0;
    long long i=strtoll( s, &e, 0);
    if( errno == ERANGE || 
        i==LONG_LONG_MAX || i==LONG_LONG_MIN 
        || ( e && *e && ( e==s || e==(s+1)))
    )
    {
        PANICV( "strtoll failed on [%s]", s );
    }
    errno=olderr;
    return i;
}

long double          
Convert::convertToLongDouble( const char *s )
{
//@@TODO: The proper way is to use configure to determine this, but I'll do
//@@TODO: that later.
#ifdef __sparc
	long double ld;

	int numConv = sscanf(s, "%Lg", &ld);

	if (numConv == 0)
	{
        PANICV( "sscanf failed on [%s]", s );
	}

	return ld;
#else
    checkNull(s);
    char *e;
    int olderr=errno;
    errno=0;
    long double d=strtold( s, &e);
    if( errno == ERANGE 
        || d==LDBL_MAX || d==LDBL_MIN
        || ( e && *e && ( e==s || e==(s+1))))
    {
        PANICV( "strtold failed on [%s]", s );
    }
    errno=olderr;
    return d;
#endif
}


unsigned long int
Convert::convertToUnsigned( const char *s )
{
    char *e;
    const char *skeep=s;
    unsigned long int u =strtoul( s, &e, 0 );
    if( u==ULONG_MAX ||         // - ULONG_MAX is always error.
        ( u==0 &&               // - 0 is only an error if...
            ( 
                e==skeep ||       //      - no characters processed or...
                errno == EINVAL   //     - errno set to EINVAL
            )
        )
    )
    {
        PANICV( "strtoul unable to convert [%s]", s );
    }
    return u;
}

unsigned long long int
Convert::convertToUnsignedLongLong( const char *s )
{
    char *e;
    const char *skeep=s;
    unsigned long long int u =strtoull( s, &e, 0 );
    if( u==ULONG_LONG_MAX ||        // - ULLONG_MAX is always error.
        ( u==0 &&               // - 0 is only an error if...
            ( 
                e==skeep ||       //      - no characters processed or...
                errno == EINVAL   //     - errno set to EINVAL
            )
        )
    )
    {
        PANICV( "strtoull unable to convert [%s]", s );
    }
    return u;
}

void
Convert::BoolToString( char *s, bool op )
{
    *(s+1)='\0';
    *s = (op) ? '1' : '0' ;
}

void
Convert::IntToString( char *s, int op )
{
    snprintf( s, 31, "%i", op );
    *(s+31)='\0';
}

void
Convert::LongToString( char *s, long int op )
{
    snprintf( s, 31, "%li", op );
    *(s+31)='\0';
}

void
Convert::DoubleToString( char *s, double op )
{
    snprintf( s, 31, "%.12g", op );
    *(s+31)='\0';
    ;
}

void
Convert::LongLongToString( char *s, long long op )
{
    snprintf( s, 31, "%lli", op );
    *(s+31)='\0';
}

void
Convert::LongDoubleToString( char *s, long double op )
{
    snprintf( s, 63, "%.61Lg", op );
    *(s+63)='\0';
}

void
Convert::UnsignedToString( char *s, unsigned int op )
{
    snprintf( s, 31, "%u", op );
    *(s+31)='\0';
}

