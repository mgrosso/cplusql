// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      frame_config.h
// Author:    mgrosso Matthew E Grosso
// Created:   Fri Dec 26 22:58:35 EST 2003 on erasmus.erasmus.org
// Project:   
// Purpose:   This file wraps the autconf generated config.h adding conditional
//              logic based on the features it found. we dont use include guards
//              in this file. instead each logical section implements its own
//              mini include guard. most logical sections are only used if 
//              NEED_foo is defined before sourcing this file.  This mechanism
//              allows multiple headers to source this file without generating
//              compiler warnings and without creating unnecessary external 
//              header file dependencies.
// 
// Copyright (c) 2003 Matthew E Grosso. All Rights Reserved.
// 
// $Id: frame_config.h,v 1.11 2005/02/06 00:21:11 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DID_CONFIG_H
#include "system_config.h"
#define DID_CONFIG_H
#endif

//get size_t
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

// ----------------------------------------------------------------------
// how many times do we receive eintr in a row before we figure out that 
// the EINTR is caused by the read/write we are doing generating sigpipe?
// ----------------------------------------------------------------------
#define MAX_CONSECUTIVE_EINTR 100

// ----------------------------------------------------------------------
// man page says use at least 26 bytes for ctime r and 
// ctime_r makes no provision for accepting length of buf :-/ 
// ----------------------------------------------------------------------
#define CTIME_BUF_SIZE 52
#define CTIME_MIN_BUF_SIZE 26

#ifdef NEED_GETTIMEOFDAY
#ifdef HAVH_TIME_H
#include <time.h>
#endif /* ifdef HAVH_TIME_H */
#include <sys/time.h>
#endif /* #ifdef NEED_GETTIMEOFDAY */


// ----------------------------------------------------------------------
// fsync isnt defined on mingw/w32
// ----------------------------------------------------------------------
#ifdef _WIN32
#define fsync( fd )  _commit( fd )
#define NEWLINE "\n\r"
#else
#define NEWLINE "\n"
#endif /* _WIN32 */

// ----------------------------------------------------------------------
// allow for more convenient testing of FRAME_USES_ACE using just ifdef
// ----------------------------------------------------------------------
#ifdef FRAME_USES_ACE
#if FRAME_USES_ACE == 0 
#undef FRAME_USES_ACE 
#else
//on RH linux 7.1, including ace/OS.h without including string.h first
//breaks.
#ifdef HAVE_STRING_H
#include "string.h"
#endif /* HAVE_STRING_H */
#include "ace/OS.h"
// #define sleep( n ) ACE_OS::sleep( n )
#endif
#endif


// ----------------------------------------------------------------------
// pthreads not supported on mingw/win32
// ----------------------------------------------------------------------
#ifdef NEED_THREADS
typedef unsigned long               dbg_thread_id_t;
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
typedef ACE_THR_FUNC_RETURN         frame_thread_return_t ;
#define FRAME_THREAD_DEFAULT_RETURN 0
typedef ACE_thread_t                frame_thread_t;
typedef ACE_mutex_t                 frame_mutex_t ;
typedef ACE_cond_t                  frame_cond_t  ;
#define GET_ULONG_THR_SELF()        (unsigned long) ACE_OS::thr_self()
#else
#include <pthread.h>
typedef void *                      frame_thread_return_t ;
#define FRAME_THREAD_DEFAULT_RETURN NULL
typedef pthread_t                   frame_thread_t;
typedef pthread_mutex_t             frame_mutex_t ;
typedef pthread_cond_t              frame_cond_t  ;
#define GET_ULONG_THR_SELF()        (unsigned long) pthread_self()

//the following symbols are defined by ACE_OS, and also defined here
//so that our code can void ifdefs.
#define THR_CANCEL_DISABLE          PTHREAD_CANCEL_DISABLE
#define THR_CANCEL_ENABLE           PTHREAD_CANCEL_ENABLE
#define THR_CANCEL_DEFERRED         PTHREAD_CANCEL_DEFERRED
#define THR_CANCEL_ASYNCHRONOUS     PTHREAD_CANCEL_ASYNCHRONOUS

#endif /* FRAME_USES_ACE */

typedef frame_thread_return_t       (*frame_thread_function)(void *);
//if, after including ace/OS.h, and pthread.h, we still dont have this, 
//make it up.
#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX 32767
#endif

#endif /* NEED_THREADS */



// ----------------------------------------------------------------------
// foo.h: example of how to add new wrapper sections around .h files here.
// ----------------------------------------------------------------------
// #ifdef NEED_FOO_H
// #ifdef HAVE_FOO_H
// #include <foo.h>
// #else
// #error you dont seem to have foo.h
// #endif /* HAVE_FOO_H */
// #undef NEED_FOO_H
// #endif /* NEED_FOO */

// ----------------------------------------------------------------------
// glob.h has extra useful funtionality on gnu libc systems.
// ----------------------------------------------------------------------
#ifdef NEED_GLOB_H
#ifdef HAVE_GLOB_H
#include <glob.h>
#ifdef HAVE_GNU_GLOB
#define FRAME_GLOB_OPTS GLOB_ERR | GLOB_NOSORT | GLOB_BRACE | GLOB_TILDE
#else
#define FRAME_GLOB_OPTS GLOB_ERR | GLOB_NOSORT
#endif /* HAVE_GNU_GLOB */
#endif /* HAVE_GLOB_H */
#undef NEED_GLOB_H
#endif /* NEED_GLOB_H */


// ----------------------------------------------------------------------
// hash_map is a nightmare, since its not standardized yet.
// ----------------------------------------------------------------------
#ifdef NEED_HASH_MAP
#if defined(HAVE_EXT_HASH_MAP)
#include <ext/hash_map>
#else
#if defined(HAVE_HASH_MAP)
#include <hash_map>
#else
#if defined(HAVE_HASH_MAP_H)
#include <hash_map.h>
#else
#include <unordered_map>
#endif /* HAVE_HASH_MAP_H */
#endif /* HAVE_STL */
#endif /* HAVE_EXT_HASH_MAP */
#ifdef HAVE_GNU_CXX_HASH_MAP
using namespace __gnu_cxx ;
#endif /* HAVE_GNU_CXX_HASH_MAP */
#undef NEED_HASH_MAP
#endif /* NEED_HASH_MAP_H */

// ----------------------------------------------------------------------
// limits.h
// ----------------------------------------------------------------------
#ifdef NEED_LIMITS_H
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif /* HAVE_LIMITS_H */
#ifndef LONG_LONG_MAX
#ifndef __LONG_LONG_MAX__
#define __LONG_LONG_MAX__ 9223372036854775807LL
#endif
#undef LONG_LONG_MIN
#define LONG_LONG_MIN (-LONG_LONG_MAX-1)
#undef LONG_LONG_MAX
#define LONG_LONG_MAX __LONG_LONG_MAX__
#endif /* LONG_LONG_MAX */
#undef NEED_LIMITS_H
#endif /* NEED_LIMITS_H */

// ----------------------------------------------------------------------
// stat() macros, S_ISLNK may not exist
// 
// since on win32 there are no soft links, its pretty safe to define 
// the S_ISLNK() macro as always being zero.
// ----------------------------------------------------------------------
#if defined (_WIN32) && defined(S_ISDIR) && ! defined (S_ISLNK)
#define S_ISLNK(i)  (0)
#endif /* (_WIN32) && (S_ISDIR) && ! (S_ISLNK) */


// ----------------------------------------------------------------------
// open() flags.
// 
// sometimes you have LARGEFILE, sometimes you dont. etc..
// ----------------------------------------------------------------------
#if defined (NEED_OPEN_FLAGS) 
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */
#ifdef HAVE_FCNTL_H
#include <sys/fcntl.h>
#endif /* HAVE_FCNTL_H */

#ifdef O_LARGEFILE
#define LARGEFILE_FLAG O_LARGEFILE
#else 
#define LARGEFILE_FLAG 0
#endif /* O_LARGEFILE */

#define OPEN_FOR_WRITING  O_CREAT | O_TRUNC | O_RDWR  | LARGEFILE_FLAG 
#define OPEN_FOR_READING  O_RDONLY  | LARGEFILE_FLAG 

#endif /* (NEED_OPEN_FLAGS) */

// ----------------------------------------------------------------------
// oracle and postgres 
// ----------------------------------------------------------------------
#ifndef HAVE_OCI_H
#undef FRAME_USES_ORACLE
#endif /* HAVE_OCI_H */
#ifndef HAVE_LIBPQ_FE_H
#undef FRAME_USES_POSTGRES
#endif  /* HAVE_LIBPQ_FE_H */

// ----------------------------------------------------------------------
// regex, use pcre (perl compatible regex) posix emulation if available
// ----------------------------------------------------------------------
#if defined( NEED_REGEX)
#ifdef HAVE_PCREPOSIX_H
#include <pcreposix.h>
#include <pcre.h>
#else 
#ifdef HAVE_REGEX_H
#include <regex.h>
#else
#error "Autoconf cant find <pcreposix.h> or <regex.h>. Either fix the include paths, or install PCRE on your system, or if you really want a crippled version of cplusql you could hack RegexWrap.cpp to stub out that functionality for this environment (but why would you ever want that ?)"
#endif
#endif
#endif

