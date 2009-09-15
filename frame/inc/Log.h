// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Log.h
// Author:    Matt Grosso
// Created:   Mon Nov  9 02:46:35 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Log.h,v 1.4 2004/07/08 16:56:57 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef LOG_H
#define LOG_H 1

#include "Dest.h"

class LogPriv ;

#define FRAMELOG_MAX_MESSAGE  2048

class Log 
{
public:
    enum elevel { 
        log_debug=0, 
        log_info=1, 
        log_warn=2, 
        log_alarm=3, 
        log_abort=4, 
        log_ignore_all=5
    };

    Log();
    Log( pDest &d );
    Log( int fd );
    Log( const char *filename );
    virtual ~Log();

    void useFile( int fd );
    void useFile( const char *filename );
    void useDest( pDest &d );

    //warning: async writes requires heap based memory allocation.
    //if you're logging a warning that you've run out of heap, this
    //might end badly.
    void setAsyncWrites( bool async, int qsize );

    void setSyslog( const char *ident, int options, int facility  );
    void setMinLogLevel( elevel min_log_level = log_info );
	elevel getMinLogLevel() const;
    void setMinSyslogLevel( elevel min_syslog_level = log_ignore_all );

    void write( elevel lev, const char *s );
    virtual void print( elevel lev, const char *fmt, ... );

    void shutdownFlush();

protected:

private:

    LogPriv *d_ ;
//NOT IMPLEMENTED 
    Log(const Log &);
    Log& operator =(const Log &);
};


#endif /* LOG_H */
