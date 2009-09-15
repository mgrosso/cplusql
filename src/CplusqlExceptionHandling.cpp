#ident "file_id $Id: CplusqlExceptionHandling.cpp,v 1.3 2005/10/17 11:18:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CplusqlExceptionHandling.cpp
// Author:    mgrosso 
// Created:   Thu Jul  8 11:49:24 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: CplusqlExceptionHandling.cpp,v 1.3 2005/10/17 11:18:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdio.h>
#include "cplusql_api.h"
#include "CplusqlExceptionHandling.h"
#include "AppLog.h"

 
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
void
cplusql_catch_lf( 
    const char *file, int line, const char *funcname, 
    Service::ServiceException &se) 
{
    APPLOG_ABORT( 
        "%s:%i %s() cplusql script line %u: caught Service::ServiceException[%s][%s]:%s", 
        file, line, funcname, cplusql_get_line(),
        se._rep_id(), se._name(), se.what_string.in());
    exit(1);
}

void
cplusql_catch_lf( 
    const char *file, int line, const char *funcname, 
    CORBA::Exception &ce )
{
    APPLOG_ABORT( 
        "%s:%i %s() cplusql script line %u: caught CORBA::Exception [%s][%s]", 
        file, line, funcname, cplusql_get_line(),
        ce._rep_id(), ce._name());
    exit(1);
}
#endif
#endif

void 
cplusql_catch_lf( 
    const char *file, int line, const char *funcname, 
    std::exception &e )
{
    APPLOG_ABORT( "%s:%i %s() cplusql input line %u: std::exception:%s\n", 
        file, line, funcname, cplusql_get_line(), e.what());
    exit(1);
}

void 
cplusql_catch_lfbe( 
    const char *file, int line, const char *funcname, 
    BaseException &be )
{
    APPLOG_ABORT( "%s:%i %s() cplusql input line %u: BaseException:%s\n", 
        file, line, funcname, cplusql_get_line(), be.what());
    exit(1);
}

void 
cplusql_catch_lf( const char *file, int line, const char *funcname )
{
    APPLOG_ABORT( "%s:%i %s() cplusql input line %u: unknown exception\n",
        file, line, funcname, cplusql_get_line());
    exit(1);
}
