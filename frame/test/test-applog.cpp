#ident "file_id $Id: test-applog.cpp,v 1.1 2004/01/25 02:15:57 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-applog.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Jan 24 18:24:19 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: test-applog.cpp,v 1.1 2004/01/25 02:15:57 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AppLog.h"

void logsome()
{
    APPLOG_DBG( "lets debug." );
    APPLOG_DBG( "lets debug w one string(%s)", "foo" );
    APPLOG_DBG( "lets debug w two strings(%s,%s)", "fu", "bar" );
    APPLOG_INFO( "lets info w one string, one int(%s,%i)", "fu", 42 );
    APPLOG_WARN( "lets alarm w no string." );
    APPLOG_ALARM( "lets alarm w no string." );
    APPLOG_ABORT( "lets abort w no strings.");
    APPLOG_ABORT( "lets abort w two strings(%s,%s)", "fu", "bar" );
}

int main( int argc, char **argv )
{
    try{
        logsome();
        Config c;
        c.addString( "APPLOG_DEST", "test-applog-test.out", true );
        c.addString( "APPLOG_MINLOG", "0", true );
        c.addString( "APPLOG_MINSYSLOG", "2", true );
        AppLog::instance()->reconfigure( c );
        logsome();
        return 0;
    }catch( exception &e )
    {
        fprintf(stderr, "\nexception caught in main: %s\n\r\n\r", e.what());
    }
    return 1;
};
