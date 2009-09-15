// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AppLog.h
// Author:    Matt Grosso
// Created:   Sun Dec 27 14:37:06 EST 1998 on circe
// Project:   libframe
// Purpose:   easy to configure and use application singleton log.
//      Add singleton semantics, and Config properties and some convenience
//      macros for logging and aborting.
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: AppLog.h,v 1.7 2005/01/05 01:07:01 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef APPLOG_H
#define APPLOG_H 1

#include "Config.h"
#include "Log.h"
class AppLog : public Log
{
public:
    static AppLog *instance();
	static void shutdown();
    void reconfigure( Config &c );
    void print(
        char *external_buf,
        const char *file,
        int line,
        const char *func,
        Log::elevel level,
        const char *msg_format,
        ...
    );
    //void abort( exception &be, ... );

private:
    static void init();
    Config c_ ;

    static const char * levelstr[6] ;
    static AppLog *instance_ ;
    AppLog();
    virtual ~AppLog();
};

#define APPLOG( level_int, ... ) \
    AppLog::instance()->print( NULL, \
    __FILE__, __LINE__, __func__, level_int, __VA_ARGS__ )

#define APPLOG_DBG( ... ) \
    APPLOG( Log::log_debug, __VA_ARGS__ )

#define APPLOG_INFO( ... ) \
    APPLOG( Log::log_info, __VA_ARGS__ )

#define APPLOG_WARN( ... ) \
    APPLOG( Log::log_warn, __VA_ARGS__ )

#define APPLOG_ALARM( ... ) \
    APPLOG( Log::log_alarm, __VA_ARGS__ )

#define APPLOG_ABORT( ... ) \
    APPLOG( Log::log_abort, __VA_ARGS__ )

//string valued config key for config that determines filename to which to write.
#define APPLOG_DEST_CFKEY "APPLOG_DEST"     
#define APPLOG_DEST_DFLT  "-"

//boolean valued config key for config that determines if writes block the caller
//until written to disk.
#define APPLOG_ASYNC_CFKEY "APPLOG_ASYNC"
#define APPLOG_ASYNC_DFLT  "no"

//integer valued config key for config that determines how many log events
//can be queued before log submissions start to block the caller.
#define APPLOG_ASYNC_QSZ_CFKEY "APPLOG_ASYNC_QSZ"
#define APPLOG_ASYNC_QSZ_DFLT  "1024"

//integer valued config key for config that determines how important
//an event has to be to log it.
#define APPLOG_MINLOG_CFKEY "APPLOG_MINLOG"
#define APPLOG_MINLOG_DFLT  "2"

//integer valued config key for config that determines how important
//an event has to be to go to syslog.
#define APPLOG_MINSYSLOG_CFKEY "APPLOG_MINSYSLOG"
#define APPLOG_MINSYSLOG_DFLT  "5"

//string valued config key for config that determines ident property
//of openlog() call, or similar on windows.
#define APPLOG_SYSLOGIDENT_CFKEY "APPLOG_SYSLOG_IDENT"
#define APPLOG_SYSLOGIDENT_DFLT  "applog"

//
//integer valued config key for config that determines what options
//to pass to openlog(). The default option is not defined here, so 
//that client does not need to source <syslog.h>.
//
#define APPLOG_SYSLOGOPTIONS_CFKEY "APPLOG_SYSLOG_OPTIONS"
// #define APPLOG_SYSLOGOPTIONS_DFLT  LOG_PID

//
//integer valued config key for config that determines what facility
//to pass to openlog(). The default option is documented but not defined
//here, so that client does not need to source <syslog.h>
//
#define APPLOG_SYSLOGFACILITY_CFKEY "APPLOG_SYSLOG_FACILITY"
// #define APPLOG_SYSLOGFACILITY_DFLT  LOG_LOCAL0

//
//integer valued config keys for config that determines what syslog level will
//be used for the batch level alarm and abort.  The default option is
//documented but not defined here, so that client code does not need to source
//<syslog.h> unless it wants to change that default.
//
#define APPLOG_SYSLOGABORTLVL_CFKEY "APPLOG_SYSLOGABORTLVL" 
//#define APPLOG_SYSLOGABORTLVL_DFLT LOG_ALERT
#define APPLOG_SYSLOGALARMLVL_CFKEY "APPLOG_SYSLOGALARMLVL" 
//#define APPLOG_SYSLOGALARMLVL_DFLT LOG_ALERT

#endif /* APPLOG_H */
