static const char * JobExecServiceClient_rcs_id = "$Id: JobExecClient.cpp,v 1.7 2004/03/29 17:10:24 holahrei Exp $";
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      JobExecServiceClient.cpp
// Author:    mgrosso Matt Grosso
// Created:   Mon Mar  1 15:27:13 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: JobExecClient.cpp,v 1.7 2004/03/29 17:10:24 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

//frame includes
#include "AppConfig.h"
#include "ConfigDump.h"

//idl generated includes
#include "JobExecServiceC.h"

//cplusql includes
#include "SharedServiceMain.h"
#include "NVPairUtil.h"
#include "keys.h"

int
main( int argc, char **argv )
{
    try{
        SharedServiceMain::init( argc, argv, __FILE__, false );
        const char * jes_url = AppConfig::instance()->getString( "URL" );
        const char * jobid = AppConfig::instance()->getString( "JOBID", "testjobid" );
        bool exclusive = AppConfig::instance()->getBool( "EXCLUSIVE", "0" );

        bool killemall = AppConfig::instance()->getBool( "DOKILL", "0" );
        bool rmstale = AppConfig::instance()->getBool( "DORMSTALE", "0" );
        bool shutdown = AppConfig::instance()->getBool( "DOSHUTDOWN", "0" );
        bool status = AppConfig::instance()->getBool( "DOSTATUS", "0" );

        const char * command = AppConfig::instance()->getString( "COMMAND", "" );

        APPLOG_DBG("doing resolv()...");
        CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve( jes_url );
        if( !obj )
        {
            PANICV( "could not resolv url: %s.", jes_url  );
        }
        APPLOG_DBG("doing narrow()...");
        JobExec::JobExecService_ptr jesp = JobExec::JobExecService::_narrow( obj );
        if( !jesp )
        {
            PANICV( "could not narrow object: %s.", jes_url  );
        }
        APPLOG_DBG("doing ping()...");
        jesp->ping();
        APPLOG_DBG("done  ping()");

        if( command && *command )
        {
            JobQueue::JobQueueRef ref;
            JobQueue::JobSubmissionStruct jobSubmission ;
            jobSubmission.jobId = jobid ;
            jobSubmission.args.command = CORBA::string_dup( command );
            jobSubmission.args.exclusive = exclusive ;
            APPLOG_DBG("running command [%s]", command );
            bool ok = jesp->runJob( ref, jobSubmission );
            APPLOG_DBG("submit returned %i", ok);
        }

        if( killemall )
        {
            APPLOG_DBG("running killAllRunning()" );
            jesp->killAllRunning();
            APPLOG_DBG("done killAllRunning()" );
        }

        if( rmstale )
        {
            APPLOG_DBG("running cleanupStaleOutput()" );
            jesp->cleanupStaleOutput();
            APPLOG_DBG("done cleanupStaleOutput()" );
        }

        if( status )
        {
            APPLOG_DBG("running pullStatus()" );
            ::Service::ServiceStatus *stat= jesp->pullStatus();
            if( stat )
            {
                APPLOG_DBG("pullStatus()  serviceName %s, url %s, hostname %s, ior %s" ,
                    stat->ref.serviceName.in(),
                    stat->ref.url.in(),
                    stat->ref.hostname.in(),
                    stat->ref.ior.in()
                );
                APPLOG_DBG("pullStatus()  status::pid %lu, is shutdown pending = %i, is live = %i",
                    static_cast< unsigned long > (stat->pid),
                    stat->isPendingShutdown,
                    stat->live
                );
                string cfgstr ;
                ConfigPtr c=NVPairUtil::convert( stat->details );
                ConfigDump::dumpToString( *c, cfgstr );
                APPLOG_DBG("pullStatus()  ConfigDump: %s", cfgstr.c_str() );
            }
            else
            {
                PANICV("pullStatus()  returned NULL!");
            }
        }

        if( shutdown )
        {
            APPLOG_DBG("running gracefulShutdown()" );
            jesp->gracefulShutdown();
            APPLOG_DBG("done gracefulShutdown()" );
        }

        return 0;
    }
    catch (CORBA::Exception &ce) 
    {
        //AppExceptionHandler really needs to learn about CORBA...
        APPLOG_ABORT("JobExecClient main() caught CORBA::Exception [%s][%s]", 
            ce._rep_id(), ce._name() );
        exit(1);
    }
    catch (std::exception &e) 
    {
        AppExceptionHandler::instance()->handleExit( e );
    }
    catch ( ... )
    {
        AppExceptionHandler::instance()->handleExit();
    }
    return 1;
}

