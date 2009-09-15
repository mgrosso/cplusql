// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      dbg.h
// Author:    Matt Grosso
// Created:   Wed Nov  4 01:16:47 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: dbg.h,v 1.3 2004/01/16 21:12:44 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DBG_H
#define DBG_H 1

#ifdef DEBUG
#include <stdio.h>
#include <unistd.h>
#include "Log.h"
#include "pthread.h"
#define FDBG(classname, message, number ) \
    fprintf( stderr, "file %s line %i class %s msg %s n %lx",  \
    __FILE__, __LINE__, \
    classname, message, number );

/*
    (Log::instance(2))->print( "file %s line %i class %s msg %s n %lx",  \
    __FILE__, __LINE__, \
    classname, message, number );
*/

/*
//#define FDBG_S( s ) \
//    (Log::instance(2))->print( "file %s line %i msg %s",  \
//    __FILE__, __LINE__, s )
*/
#define FDBG_S( s ) \
    fprintf( stderr, "%li:%i:%s:%i [%s]\n",  \
    pthread_self(), getpid(), __FILE__, __LINE__, s )

#define FDBG_SS( s, s2 ) \
    fprintf( stderr, "%li:%i:%s:%i [%s] [%s]\n",  \
    pthread_self(), getpid(), __FILE__, __LINE__, s, s2 )

/*
//#define FDBG_SI( s, n ) \
//    (Log::instance(2))->print( "file %s line %i msg %s n %i",  \
//    __FILE__, __LINE__, s, n )
*/

#define FDBG_SI( s, n ) \
    fprintf( stderr, "%li:%i:%s:%i [%s] %#0lx\n",  \
    pthread_self(), getpid(), __FILE__, __LINE__, s, (long unsigned )n )

#define FDBG_SII( s, n1, n2 ) \
    fprintf( stderr, "%li:%i:%s:%i [%s] %#0lx - %#0lx\n",  \
    pthread_self(), getpid(), __FILE__, __LINE__, s, (long unsigned )n1, (long unsigned )n2 )

#define FDBG_SSI( s, s2, n ) \
    fprintf( stderr, "%li:%i:%s:%i [%s] [%s] %#0lx\n",  \
    pthread_self(), getpid(), __FILE__, __LINE__, s, s2, (long unsigned )n )

#define FDBG_SSII( s, s2, n1, n2 ) \
    fprintf( stderr, "%li:%i:%s:%i [%s] [%s] %#0lx - %#0lx\n",  \
    pthread_self(), getpid(), __FILE__, __LINE__, s, s2,  \
    (long unsigned )n1, (long unsigned)n2 )

#else
#define FDBG(classname, message, number )
#define FDBG_S( s )
#define FDBG_SS( s, s2 )
#define FDBG_SI( s, n )
#define FDBG_SSI( s, s2, n )
#define FDBG_SII( s, n1, n2 )
#define FDBG_SSII( s, s2, n1, n2 )
#endif /* DEBUG */
#endif /* DBG_H */
