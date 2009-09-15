#ident "file_id $Id: AppLog.cpp,v 1.17 2005/04/25 17:47:06 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AppLog.cpp
// Author:    Matt Grosso
// Created:   Tue Mar 23 01:21:29 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Matt Grosso.  All Rights Reserved.
// 
// $Id: AppLog.cpp,v 1.17 2005/04/25 17:47:06 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_THREADS 

#include <unistd.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>

#include "frame_config.h"
#include "AppLog.h"
#include "Config.h"
#include "twrap.h"


const char * AppLog::levelstr[6] = {
    "debug","info","warn","alarm","abort","ignore"
};
static pthread_once_t applog_is_initialized = PTHREAD_ONCE_INIT;

AppLog *AppLog::instance_ ;

void AppLog::init()
{
    instance_ = new AppLog();
}

AppLog *
AppLog::instance()
{
    (void)pthread_once(&applog_is_initialized, AppLog::init ); 
    return instance_;
}

void
AppLog::reconfigure( Config &c )
{
    c_.slurp( c, true );
    const char *filename = c.getString( APPLOG_DEST_CFKEY, APPLOG_DEST_DFLT );
    if( *filename == '-' )
    {
        useFile( 2 );
    }
    else
    {
        useFile( filename );
    }
    setAsyncWrites(
        c.getBool( APPLOG_ASYNC_CFKEY, APPLOG_ASYNC_DFLT ),
        c.getInt( APPLOG_ASYNC_QSZ_CFKEY, APPLOG_ASYNC_QSZ_DFLT )
    );
    setMinLogLevel( 
        static_cast<Log::elevel>(
            c.getInt( APPLOG_MINLOG_CFKEY, APPLOG_MINLOG_DFLT )
        )
    );
    setMinSyslogLevel( 
        static_cast<Log::elevel>(
            c.getInt( APPLOG_MINSYSLOG_CFKEY, APPLOG_MINSYSLOG_DFLT )
        )
    );
    setSyslog( 
        c.getString( APPLOG_SYSLOGIDENT_CFKEY, APPLOG_SYSLOGIDENT_DFLT ),
        c.getInt( APPLOG_SYSLOGOPTIONS_CFKEY, "0" ),
        c.getInt( APPLOG_SYSLOGFACILITY_CFKEY, "0" )
    );
}

AppLog::AppLog()
{
    c_.loadEnv();
    reconfigure( c_ );
}

AppLog::~AppLog()
{
	; //noop
}


void
AppLog::shutdown()
{
	if (instance_ != (AppLog *) 0)
	{
		instance_->shutdownFlush();
		delete instance_;
		instance_ = (AppLog *) 0;
	}
}


void
AppLog::print( 
        char *external_buf,
        const char *file,
        int line,
        const char *func,
        Log::elevel level,
        const char *msg_format,
        ...
        )
{
    char ubuf[FRAMELOG_MAX_MESSAGE];
    char buf[FRAMELOG_MAX_MESSAGE];
    char ctime_rbuf[CTIME_BUF_SIZE];

	memset( ubuf, '\0', FRAMELOG_MAX_MESSAGE );
	memset( buf, '\0', FRAMELOG_MAX_MESSAGE );
	memset( ctime_rbuf, '\0', CTIME_BUF_SIZE );
    if( external_buf )
    {
        memset( external_buf, '\0', FRAMELOG_MAX_MESSAGE );
    }

    suseconds_t susec;
    twrap::getctime( ctime_rbuf, CTIME_BUF_SIZE, susec );

    
    if( level < 0 || level > Log::log_ignore_all )
    {
        level = Log::log_ignore_all;
    }
    const char *level_s = levelstr[level];
    
    va_list ap;
    va_start(ap, msg_format );
    int numWr = vsnprintf( ubuf, FRAMELOG_MAX_MESSAGE-1, msg_format, ap );
    va_end( ap );

	int numWr2 = snprintf(
        external_buf ?  external_buf : buf,
        FRAMELOG_MAX_MESSAGE-1,
        "%s pid=%li,ppid=%li,thr=%lx(%s)%li %s:%i %s() msg(%s)",
		level_s, 
        (long) getpid(),
        (long) getppid(),
        GET_ULONG_THR_SELF(), 
        ctime_rbuf,
        susec,
        file,
        line,
        func,
        ubuf
    );
    if( NULL == external_buf )
    {
        write( level, buf );
		if (numWr >= FRAMELOG_MAX_MESSAGE-1)
		{
			(void) ::snprintf(buf, FRAMELOG_MAX_MESSAGE-1, "%s pid=%li,ppid=%li,thr=%lx(%s)%li %s:%i %s() message too big, vsnprintf skipped %ld bytes",
				levelstr[Log::log_warn],
				(long) getpid(),
				(long) getppid(),
				GET_ULONG_THR_SELF(),
				ctime_rbuf,
				susec,
				__FILE__,
				__LINE__,
				func,
				(long) (numWr - (FRAMELOG_MAX_MESSAGE - 1))
			);
			write( Log::log_warn, buf);
		}
		if (numWr2 >= FRAMELOG_MAX_MESSAGE-1)
        {
			(void) ::snprintf(buf, FRAMELOG_MAX_MESSAGE-1, "%s pid=%li,ppid=%li,thr=%lx(%s)%li %s:%i %s() message too big; snprintf skipped %ld bytes",
				levelstr[Log::log_warn],
				(long) getpid(),
				(long) getppid(),
				GET_ULONG_THR_SELF(),
				ctime_rbuf,
				susec,
				__FILE__,
				__LINE__,
				func,
				(long) (numWr2 - (FRAMELOG_MAX_MESSAGE - 1))
			);
			write( Log::log_warn, buf);
        }

    }
};

