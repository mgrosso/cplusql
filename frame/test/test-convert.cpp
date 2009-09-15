#ident  "file_id $Id: test-convert.cpp,v 1.3 2004/08/11 21:42:44 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-convert.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 16:49:39 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-convert.cpp,v 1.3 2004/08/11 21:42:44 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "Convert.h"
#include "BaseException.h"
#include <stdio.h>

static int error_count = 0;

void
tryall( const char *s )
{
    bool b=false;
    long int i=0;
    double d=0;
    long long int li=0;
    long double ld=0;

    const char *ds= s ? s : "NULL";
    fprintf( stderr, "testing %s\n", ds );

    bool except=false;
    try {
        b=Convert::convertToBool(s);
    }catch( BaseException &beb )
    {
        except=true;
    }
    fprintf( stderr, "b[]: %s %i\n", except ? "exception" : "", b );

    except=false;
    try {
        i=Convert::convertToLong(s);
    }catch( BaseException &bei )
    {
        except=true;
    }
    fprintf( stderr, "i[]: %s %li\n", except ? "exception" : "", i );

    except=false;
    try {
        d=Convert::convertToDouble(s);
    }catch( BaseException &bed )
    {
        except=true;
    }
    fprintf( stderr, "d[]: %s %g\n", except ? "exception" : "", d );

    except=false;
    try {
        li=Convert::convertToLongLong(s);
    }catch( BaseException &beli )
    {
        except=true;
    }
    fprintf( stderr, "ll[]: %s %lli\n", except ? "exception" : "", li );

    except=false;
    try {
        ld=Convert::convertToLongDouble(s);
    }catch( BaseException &beld )
    {
        except=true;
    }
    fprintf( stderr, "ld[]: %s %Lg\n\n", except ? "exception" : "", ld );
}

void
checkexcept( const char *t, bool did, bool shouldhave, const char *s )
{
    if( did != shouldhave )
    {
        ++error_count;
        fprintf( stderr, "%i Convert::convertTo%s(%s): should have thrown?[%i] did throw?[%i]\n", 
            error_count, t, s, shouldhave, did );
    }
}

void
checkall(
    const char *    s,
    bool            boolthrows,
    bool            boolanswer,
    bool            longintthrows,
    long int        longintanswer,
    bool            doublethrows,
    double          doubleanswer,
    bool            longlongintthrows,
    long long int   longlongintanswer,
    bool            longdoublethrows,
    long double     longdoubleanswer
)
{
    bool b=false;
    long int i=0;
    double d=0;
    long long int li=0;
    long double ld=0;

    const char *ds= s ? s : "NULL";
    fprintf( stderr, "testing %s\n", ds );

    bool except=false;
    try {
        b=Convert::convertToBool(s);
    }catch( BaseException &beb )
    {
        except=true;
    }
    checkexcept( "Bool", except, boolthrows, s );
    if( !except && boolanswer != b )
    {
        ++error_count;
        fprintf( stderr, "%i Convert::convertToBool(%s): answer was %i, should be %i\n", 
            error_count, s, b, boolanswer );
    }

    except=false;
    try {
        i=Convert::convertToLong(s);
    }catch( BaseException &bei )
    {
        except=true;
    }
    checkexcept( "Long", except, longintthrows, s );
    if( !except && longintanswer != i )
    {
        ++error_count;
        fprintf( stderr, "%i Convert::convertToLong(%s): answer was %li, should be %li\n", 
            error_count, s, i, longintanswer );
    }

    except=false;
    try {
        d=Convert::convertToDouble(s);
    }catch( BaseException &bed )
    {
        except=true;
    }
    checkexcept( "Double", except, doublethrows, s );
    if( !except && doubleanswer != d )
    {
        ++error_count;
        fprintf( stderr, "%i Convert::convertToDouble(%s): answer was %g, should be %g\n", 
            error_count, s, d, doubleanswer );
    }

    except=false;
    try {
        li=Convert::convertToLongLong(s);
    }catch( BaseException &beli )
    {
        except=true;
    }
    checkexcept( "LongLong", except, longlongintthrows, s );
    if( !except && longlongintanswer != li )
    {
        ++error_count;
        fprintf( stderr, "%i Convert::convertToLongLong(%s): answer was %lli, should be %lli\n", 
            error_count, s, li, longlongintanswer );
    }

    except=false;
    try {
        ld=Convert::convertToLongDouble(s);
    }catch( BaseException &beld )
    {
        except=true;
    }
    checkexcept( "LongDouble", except, longdoublethrows, s );
    if( !except && longdoubleanswer != ld )
    {
        ++error_count;
        fprintf( stderr, "%i Convert::convertToLongDouble(%s): answer was %Lg, should be %Lg\n", 
            error_count, s, ld, longdoubleanswer );
    }
};

#define CHECKALL( sarg, iarg ) checkall( sarg, false, true, false, (long int)iarg, false, (double)iarg, false, (long long)iarg, false, (long double)iarg )
#define NOOP( i ) ( " \" " )

int
main( int argc, char **argv )
{
    checkall( NULL, true, 0, true, 0, true, 0, true, 0, true, 0 );
    checkall( "", true, 0, true, 0, true, 0, true, 0, true, 0 );
    checkall( "true", false, true, true, 0, true, 0, true, 0, true, 0 );
    checkall( "false", false, false, true, 0, true, 0, true, 0, true, 0 );
    checkall( "yes", false, true, true, 0, true, 0, true, 0, true, 0 );
    checkall( "no", false, false, true, 0, true, 0, true, 0, true, 0 );
    checkall( "0", false, false, false, 0, false, 0, false, 0, false, 0 );
    checkall( "1", false, true, false, 1, false, 1, false, 1, false, 1 );
    checkall( "-1", false, true, false, -1, false, -1, false, -1, false, -1 );
    checkall( "i am not a number!", true, false, true, 0, true, 0, true, 0, true, 0 );
    CHECKALL( "69", 69 );
    CHECKALL( "69.000", 69.000 );
    CHECKALL( "69.000000000", 69.000000000 );
    CHECKALL( "0x69", 0x69 );
    tryall( "0x69");
    tryall( "1");
    tryall( "12");
    tryall( "123");
    tryall( "1234");
    tryall( "12345");
    tryall( "123456");
    tryall( "1234567");
    tryall( "12345678");
    tryall( "123456789");
    tryall( "1234567890");
    tryall( "12345678901");
    tryall( "123456789012");
    tryall( "1234567890123");
    tryall( "12345678901234");
    tryall( "123456789012345");
    tryall( "1234567890123456");
    tryall( "12345678901234567");
    tryall( "123456789012345678");
    tryall( "1234567890123456789");
    tryall( "12345678901234567890");
    tryall( "123456789012345678901");
    tryall( "1234567890123456789012");
    tryall( "12345678901234567890123");
    tryall( "123456789012345678901234");
    tryall( "1234567890123456789012345");
    tryall( "12345678901234567890123456");
    tryall( "123456789012345678901234567");
    tryall( "1234567890123456789012345678");
    tryall( "12345678901234567890123456789");
    tryall( "123456789012345678901234567890");

    tryall( "-1");
    tryall( "-12");
    tryall( "-123");
    tryall( "-1234");
    tryall( "-12345");
    tryall( "-123456");
    tryall( "-1234567");
    tryall( "-12345678");
    tryall( "-123456789");
    tryall( "-1234567890");
    tryall( "-12345678901");
    tryall( "-123456789012");
    tryall( "-1234567890123");
    tryall( "-12345678901234");
    tryall( "-123456789012345");
    tryall( "-1234567890123456");
    tryall( "-12345678901234567");
    tryall( "-123456789012345678");
    tryall( "-1234567890123456789");
    tryall( "-12345678901234567890");
    tryall( "-123456789012345678901");
    tryall( "-1234567890123456789012");
    tryall( "-12345678901234567890123");
    tryall( "-123456789012345678901234");
    tryall( "-1234567890123456789012345");
    tryall( "-12345678901234567890123456");
    tryall( "-123456789012345678901234567");
    tryall( "-1234567890123456789012345678");
    tryall( "-12345678901234567890123456789");
    tryall( "-123456789012345678901234567890");

    return error_count;
}

