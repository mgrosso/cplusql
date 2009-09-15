#ident "file_id $Id: BaseException.cpp,v 1.12 2005/01/05 01:11:42 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BaseException.h
// Author:    Matt Grosso
// Created:   Wed Oct 14 21:15:48 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: BaseException.cpp,v 1.12 2005/01/05 01:11:42 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_THREADS
#define NEED_GETTIMEOFDAY
#include "frame_config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include "BaseException.h"
#include "twrap.h"

 
BaseException::BaseException() throw()
{
	strcpy( message_, "Exception!" );
};
 
BaseException::BaseException( const char *s) throw()
{
	strncpy( message_, s, BE_LEN );
	*(message_+BE_LEN-1)='\0';
};

BaseException::BaseException( string &s)
{
	strncpy( message_, s.c_str(), BE_LEN );
	*(message_+BE_LEN-1)='\0';
};

BaseException::BaseException(const BaseException &be) throw ()
	: exception(be)
{
	strncpy(message_, be.message_, BE_LEN);
};

BaseException &
BaseException::operator=(const BaseException &be) throw ()
{
	exception::operator=(be);
	strncpy(message_, be.message_, BE_LEN);

	return *this;
}


BaseException 
BaseException::factory( const char *fmt, ... ) throw()
{
	char msg[BE_LEN];
    memset( msg, '\0', BE_LEN );
	va_list val ;
	va_start( val, fmt );
	vsnprintf( msg, BE_LEN, fmt, val );
    va_end(val);
    hang_or_abort_on_panic(msg);
	return BaseException((const char *)msg);
};

BaseException 
BaseException::factoryv( 
    const char *file, int line, const char *func,
    const char *fmt, ... ) throw()
{
    int local_errno=errno;
    char ctime_rbuf[CTIME_BUF_SIZE];
	memset( ctime_rbuf, '\0', CTIME_BUF_SIZE );
    suseconds_t susec ;
    twrap::getctime( ctime_rbuf, CTIME_BUF_SIZE, susec );

    unsigned long mythread = GET_ULONG_THR_SELF();
    long p=static_cast<long >(getpid());
    long pp=static_cast<long >(getppid());

	char usermsg[BE_LEN];
    memset( usermsg, '\0', BE_LEN );
	va_list val ;
	va_start( val, fmt );
	vsnprintf( usermsg, BE_LEN, fmt, val );
    va_end(val);

	char osmsg[PE_LEN];
    memset( osmsg, '\0', PE_LEN );
#ifndef HAVE_STRERROR_R
	//@@TODO
	sprintf(osmsg, "%ld", (long) errno);
#else
    strerror_r( errno, osmsg, PE_LEN-1 );
#endif

	char fullmsg[BE_LEN];
    memset( fullmsg, '\0', BE_LEN );
    snprintf(
        fullmsg,
        BE_LEN-1,
        "Panic! [%s][%i] func[%s] pid,ppid[%li,%li] thr[%lu] [%s] errno[%i][%s] -> %s ", \
        file, line, func,
        p, pp, mythread, ctime_rbuf,
        local_errno, osmsg, usermsg
    );
    hang_or_abort_on_panic(fullmsg);
	return BaseException((const char *)fullmsg);
};

void 
BaseException::dump_stack_trace() throw()
{
    void * mystacktrace[200];
    size_t size = backtrace(mystacktrace,200);
    backtrace_symbols_fd( (void **)&mystacktrace, size, 2);
}

void 
BaseException::hang_or_abort_on_panic( const char *msg ) 
{
    if( getenv("STACK_TRACE_ON_PANIC"))
    {
        dump_stack_trace();
    }
    if( getenv( "ABORT_ON_PANIC" ))
    {
        fprintf( stderr, 
            "%s %i: pid %i ABORT ON PANIC: %s\n", 
            __FILE__, __LINE__, getpid(), msg 
        );
        raise(SIGABRT);
    }
    else if( getenv( "HANG_ON_PANIC" ))
    {
        fprintf( stderr, 
            "%s %i: pid %i HANG ON PANIC: %s\n", 
            __FILE__, __LINE__, getpid(), msg 
        );
#ifdef FRAME_USES_ACE 
        while(1) 
        {
            ACE_OS::sleep( 6000 );
        }
#else
        pause();
#endif
    }
};

const char * 
BaseException::getMessage() const throw()
{ 
    return message_ ;
};

const char * 
BaseException::what() const throw()
{ 
    return getMessage() ;
};

