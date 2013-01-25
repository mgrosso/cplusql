#ident "file_id: $Id: Daemon.cpp,v 1.4 2004/09/21 23:44:21 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Daemon.cpp
// Author:    mgrosso Matt Grosso
// Created:   Fri Mar 26 14:20:56 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: Daemon.cpp,v 1.4 2004/09/21 23:44:21 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "AppLog.h"
#include "DestFile.h"
#include "ptr_strdup.h"
#include "Slurp.h"
#include "Convert.h"

#include "Daemon.h"

void 
Daemon::daemonize( bool dofork, const char *homedir, const char *pidfile )
{
    if( !pidfile || !*pidfile )
    {
        PANICV( "null arg for pidfile." );
    }
    if( homedir && *homedir )
    {
        int r = chdir( homedir );
        if( r != 0 )
        {
            PANICV( "could not change dir to %s", homedir );
        }
        APPLOG_INFO("did chdir(%s).", homedir );
    }

    if( dofork )
    {
        int ret=fork();
        if( -1 == ret )
        {
            PANICV("could not fork().");
        }
        if( 0 != ret )
        {
            APPLOG_INFO("parent exits after successfull fork.");
            exit(0);
        }
        APPLOG_INFO("child continuing after successfull fork.");
    }
    else
    {
        APPLOG_INFO("not forking.");
    }

    try{
        const char *pidfile_contents = Slurp::slurpFile( pidfile );
        if( pidfile_contents && *pidfile_contents )
        {
            unsigned int prev_pid = Convert::convertToUnsigned( pidfile_contents );
            if( 0 == kill( prev_pid, 0 ))
            {
                APPLOG_INFO(
                    "child exiting because pidfile %s contains pid %lu which exists.",
                    pidfile, prev_pid);
                exit(0);
            }
        }
        APPLOG_INFO(
            "child continuing after checking that %s did not contain a running pid.",
            pidfile );
    }catch( ... )
    {
        APPLOG_INFO(
            "did not find or could not open previous pidfile %s, continuing.", 
            pidfile );
    }

    char_ptr pidstring = ptr_strdup_long( getpid());
    {
        DestFile pf ( pidfile, true );
        pf.put( pidstring.get());
        pf.put( "\n" );
    }
    APPLOG_INFO("wrote %s to pid file %s", pidstring.get(), pidfile );
    APPLOG_DBG("closing fd 0,1" );
    close(1);
    close(0);
    APPLOG_DBG("closed fd 0,1. 2, stderr, still open. If you experience hangs using rsh to start this app, you may need to redirect it to a file." );
}


