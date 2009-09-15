#ident  "file_id $Id: test-parse_rfc822.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      rfc822dir.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 14 19:57:54 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-parse_rfc822.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include <stdio.h>
#include "ptr_strdup.h"
#include "SlurpedLineSource.h"
#include "RFC822Source.h"
#include "BaseException.h"


// ----------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------
#if 0
static void 
dbg( int line, const char *file, const char *msg )
{
    if( msg == 0 )
    {
        msg="";
    }
    fprintf(stderr, "%s[%i] %s\n", file, line, msg );
};
#define BDBG( x ) dbg( __LINE__, __FILE__, x )
#else
#define BDBG( x ) 
#endif

void
usage( const char *msg = 0 )
{
    if( msg )
    {
        fprintf(stderr, "error: %s\n\n",msg );
    }
//    fprintf(stderr, "usage: rfc822dir --<key>=<value>\n" );
//    fprintf(stderr, "where <key> is one of ... \n");
//    fprintf(stderr, "\tmailfile (required, no default)\n");
    exit(1);
};


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
int 
main(int argc, char **argv )
{
    try{
        BDBG( "start" );

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
        {
            /*
            SlurpedLineSource sls( "/etc/passwd" );
            while( sls.next())
            {
                char_ptr line = sls.getCharPtr() ;
                fprintf(stderr, "%s\n", line.get());
            }
            */
        }
        {
            RFC822Source rs( "accipiter" );
            while( rs.next())
            {
                char_ptr line = rs.getCharPtr() ;
                fprintf(stderr, "MESSAGE %s\nCOMPLETE\n\n", line.get());
            }
        }
    }
    catch( BaseException &be )
    {
        usage( be.getMessage());
    }
    catch( ... )
    {
        usage( "unknown exception" );
    }
    return 0;
}




