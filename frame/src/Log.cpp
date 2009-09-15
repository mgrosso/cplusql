#ident "file_id $Id: Log.cpp,v 1.13 2004/08/10 21:22:16 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Log.cpp
// Author:    Matt Grosso
// Created:   Mon Nov  9 02:46:29 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Log.cpp,v 1.13 2004/08/10 21:22:16 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_THREADS
#include "frame_config.h"

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif /* HAVE_SYSLOG_H */

#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>
#include "BaseException.h"
#include "Log.h"
#include "Exclusion.h"
#include "Event.h"
#include "DestFile.h"
#include "ProxyEventHandler.h"
#include "dbg.h"

#define FRAMELOG_IDENT "libframe log" 
#define FRAMELOG_OPTIONS 0 
#define FRAMELOG_FACILITY LOG_USER 

typedef struct log_event
{
    log_event( Log::elevel l, bool do_delete);
    Log::elevel   level;
    bool     delete_me;
    char     buf[ FRAMELOG_MAX_MESSAGE ];
    const char *msg;
} log_event_t;

class LogPriv : public EventHandler
{
friend class Log;
public:
    virtual void handleEvent( void *loge );
            void setSyslog( const char *ident, int options, int facility );

    ~LogPriv();
    LogPriv( pDest &dest);

private:
    EventQueuePtr       eq_;
    pDest               dest_;
    Mutex               lock_;

    Log::elevel         min_log_; 
    Log::elevel         min_syslog_ ;

    static int          sysloglevels[6];

//NOT IMPLEMENTED 
    LogPriv();
    LogPriv( const LogPriv & );
    const LogPriv &operator=( const LogPriv & );
};

int LogPriv::sysloglevels[6] = { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_CRIT, LOG_ALERT, LOG_DEBUG };

log_event::log_event( Log::elevel l, bool do_delete)
:level(l),delete_me(do_delete)
{
    ;//
}

LogPriv::LogPriv( pDest &dest )
:
    dest_(dest), 
    min_log_(Log::log_info),
    min_syslog_(Log::log_ignore_all)
{
    pEventHandler pe = new ProxyEventHandler( this );
    pEventHandler pehnull = NULL;
    pEventFactory pefnull = NULL;
    eq_=new EventQueue(pefnull , pehnull,pe,0,0,INT_MAX);
    //TODO: init syslog goes here
    setSyslog( FRAMELOG_IDENT, FRAMELOG_OPTIONS, FRAMELOG_FACILITY );
}

void
LogPriv::handleEvent( void *loge )
{
    log_event_t *e=reinterpret_cast< log_event_t *>(loge);
    if( e->level >= min_syslog_ && e->level >= Log::log_debug )
    {
#ifdef HAVE_SYSLOG_H
        syslog( LogPriv::sysloglevels[e->level], e->msg);
#endif /* HAVE_SYSLOG_H */
    }
    if( e->level >= min_log_ )
    {
        //use a ptr copy of dest, so that dest can be changed 
        //while we do put() without causing object we use to 
        //be deleted from underneath us. This allows us to not
        //wrap dest in a mutex. ptr copy is constant time, while mutex
        //can block
        pDest copy_dest_ptr=dest_;
        copy_dest_ptr->put( e->msg );
        copy_dest_ptr->put( NEWLINE );
    }
    if( e->delete_me )
    {
        delete e;
    }
}

LogPriv::~LogPriv() 
{
}


Log::Log()
{
    pDest pd( new DestFile( 2 ));
    d_ = new LogPriv( pd );
};

Log::Log( int fd )
{
    pDest pd( new DestFile( fd ));
    d_ = new LogPriv( pd );
};

Log::Log( pDest &d )
{
    d_ = new LogPriv( d );
};

Log::Log( const char *filename )
{
    pDest pd( new DestFile( filename, true, false, false ));
    d_ = new LogPriv( pd );
};

Log::~Log()
{
	if (d_ != (LogPriv *) 0)
	{
	    delete d_;
		d_ = (LogPriv *) 0;
	}
}

void
Log::useFile( int fd )
{
    pDest pnew( new DestFile(fd));
    useDest(pnew);
};

void
Log::useFile( const char *filename )
{
    pDest pnew( new DestFile(filename, true, false, false));
    useDest(pnew);
};

void
Log::useDest( pDest &d )
{
    d_->dest_ = d;
};

void
Log::setAsyncWrites( bool async, int qsize )
{
    unsigned long handlers = async ? 1 : 0 ;
    if( handlers == d_->eq_->getNumHandlers() )
    {
        return ;
    }
    pEventHandler pe = new ProxyEventHandler( d_ );
    pEventHandler pehnull = NULL;
    pEventFactory pefnull = NULL;
    d_->eq_=new EventQueue(pefnull , pehnull,pe,0, handlers, qsize);
};

void
Log::setSyslog( const char *ident, int options, int facility )
{
    d_->setSyslog( ident, options, facility );
};

void
LogPriv::setSyslog( const char *ident, int options, int facility )
{
    if( min_syslog_ != Log::log_ignore_all )
    {
#ifdef HAVE_SYSLOG_H
        openlog( ident, options, facility );
#endif /* HAVE_SYSLOG_H */
    }
};

void
Log::setMinSyslogLevel( elevel minlevel )
{
    if( minlevel >= log_debug && minlevel <= log_ignore_all )
    {
        d_->min_syslog_ = minlevel;
    }
};

void
Log::setMinLogLevel( elevel minlevel )
{
    if( minlevel >= log_debug && minlevel <= log_ignore_all )
    {
        d_->min_log_ = minlevel;
    }
};

Log::elevel
Log::getMinLogLevel() const
{
	return d_->min_log_;
}


void
Log::shutdownFlush()
{
	if (d_ != (LogPriv *) 0)
	{
		if (d_->eq_.get() != (EventQueue *) 0)
		{
		    d_->eq_->gracefulShutdown();
		}
		if (d_->dest_.get() != (Dest *) 0)
		{
		    d_->dest_->flush();
		}
	}
};

void
Log::write( elevel level, const char *s ){
    if( 
        (!s) || 
        (!*s) ||
        (level < log_debug) || 
        (level > log_abort )
    )
    {
        return;
    }
    //use a ptr copy of eq, so that dest can be changed 
    //while we do put() without causing object we use to 
    //be deleted from underneath us. This allows us to not
    //wrap eq in a mutex.  ptr copy is constant time, while mutex
    //can block.
	EventQueuePtr eq = d_->eq_;

    if( eq->getNumHandlers() > 0 )
    {
        log_event_t *loge = new log_event_t( level, true );
        strncpy( loge->buf, s, FRAMELOG_MAX_MESSAGE -1 );
        loge->buf[FRAMELOG_MAX_MESSAGE-1]='\0';
        loge->msg=loge->buf;
        eq->push( static_cast< void * >(loge));
    }
    else
    {
        //synchronous call, so dont copy string. dont use heap.
        log_event_t lge( level, false );
        lge.msg= s ;
        eq->push( static_cast< void * >(& lge ));
    }
};

void
Log::print( elevel level, const char *fmt, ... )
{
    if( 
        (!fmt) || 
        (!*fmt) ||
        (level < log_debug) || 
        (level > log_abort )
    )
    {
        return;
    }
    va_list ap;
    va_start( ap, fmt );

    char buf[FRAMELOG_MAX_MESSAGE];
    memset( buf, '\0', FRAMELOG_MAX_MESSAGE );
    vsnprintf( buf, FRAMELOG_MAX_MESSAGE-1, fmt, ap );
    buf[FRAMELOG_MAX_MESSAGE-1]='\0';
    va_end( ap);
	write(level, buf );
}
