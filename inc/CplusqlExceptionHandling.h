
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CplusqlExceptionHandling.h
// Author:    mgrosso 
// Created:   Thu Jul  8 11:44:47 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: CplusqlExceptionHandling.h,v 1.2 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef CPLUSQLEXCEPTIONHANDLING_H
#define CPLUSQLEXCEPTIONHANDLING_H 1

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#include "serviceC.h"
#endif
#endif
#include "BaseException.h"

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
void
cplusql_catch_lf( 
    const char *file, int line, const char *funcname, 
    Service::ServiceException &se) ;

void
cplusql_catch_lf( 
    const char *file, int line, const char *funcname, CORBA::Exception &ce);
#endif
#endif

void 
cplusql_catch_lf( 
    const char *file, int line, const char *funcname, std::exception &e );

void 
cplusql_catch_lfbe( 
    const char *file, int line, const char *funcname, 
    BaseException &be );

void 
cplusql_catch_lf( const char *file, int line, const char *funcname );

#define CPLUSQL_CATCHALL() \
    catch( BaseException &be ){  \
        cplusql_catch_lfbe( __FILE__, __LINE__, __func__, be ); } \
    catch( std::exception &e ){  \
        cplusql_catch_lf( __FILE__, __LINE__, __func__, e ); } \
    catch( ... ){  \
        cplusql_catch_lf( __FILE__, __LINE__, __func__ ); }

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#undef CPLUSQL_CATCHALL
#define CPLUSQL_CATCHALL() \
    catch( Service::ServiceException &se ){  \
        cplusql_catch_lf( __FILE__, __LINE__, __func__, se ); } \
    catch( CORBA::Exception &ce ){  \
        cplusql_catch_lf( __FILE__, __LINE__, __func__, ce ); } \
    catch( BaseException &be ){  \
        cplusql_catch_lfbe( __FILE__, __LINE__, __func__, be ); } \
    catch( std::exception &e ){  \
        cplusql_catch_lf( __FILE__, __LINE__, __func__, e ); } \
    catch( ... ){  \
        cplusql_catch_lf( __FILE__, __LINE__, __func__ ); }
#endif
#endif




#endif /* CPLUSQLEXCEPTIONHANDLING_H */
