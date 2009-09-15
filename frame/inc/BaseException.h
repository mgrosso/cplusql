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
// $Id: BaseException.h,v 1.13 2005/01/05 01:11:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H 1

#include <stdio.h>
#include <string>
#include <exception>
#include <stdarg.h>
#include <errno.h>
#define BE_LEN 2048
#define PE_LEN 128

using namespace std;

class BaseException : public exception
{
public:
	BaseException() throw();
	BaseException( const char *c ) throw();
	BaseException( string &s );//c_str() may throw.
	BaseException(const BaseException &be) throw ();

	BaseException &operator=(const BaseException &be) throw ();

	static BaseException factory( const char *fmt, ... ) throw();
	static BaseException factoryv( 
        const char *file, int line, const char *func,
        const char *fmt, ... 
    ) throw();
    static void         dump_stack_trace() throw();
	const char *   getMessage() const throw() ;
	virtual const char *   what() const throw() ;
protected:
	mutable char message_[BE_LEN];
private:
    static void hang_or_abort_on_panic( const char *msg );
};

#define PANICV( ... ) \
    throw BaseException::factoryv( __FILE__, __LINE__, __func__, __VA_ARGS__ ) 

//obsolete macros, now that we have __VA_ARGS__ in gcc 
#define PANIC( m ) PANICV( m )
#define PANIC_I( m, i ) PANICV( m, i )
#define PANIC_S( m, s ) PANICV( m, s )
#define THROW_PANIC( m ) PANIC( m )

//doubly obsolete macros, since new will throw by itself. dont use this.
#define THROW_NOMEM PANIC( "memory allocation failure" )

#endif /* BASEEXCEPTION_H */

