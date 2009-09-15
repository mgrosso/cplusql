#ident "file_id $Id: Exclusion.cpp,v 1.6 2004/03/05 17:57:27 holahrei Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MutexGuard.h
// Author:    Matt Grosso
// Created:   Thu Oct 15 00:06:32 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Exclusion.cpp,v 1.6 2004/03/05 17:57:27 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdio.h>
#include <sys/time.h>
#include <errno.h>

#include "Exclusion.h"
#include "BaseException.h"

#ifdef DEBUG 
#include <sys/types.h>
#include <unistd.h>
#define MDB(s) \
	fprintf(stderr, \
	"[%li][%i][%s][%i][Mutex::%s] lock[%x] owner[%li]\n", \
	GET_ULONG_THR_SELF(), getpid(), __FILE__, __LINE__, \
	s, (unsigned int)&lock_, owner_ );
#define MGDB(s) \
	fprintf(stderr, \
	"[%li][%i][%s][%i][MutexGuard::%s] lock[%x] owner[%li]\n", \
	GET_ULONG_THR_SELF(), getpid(), __FILE__, __LINE__, \
	s, (unsigned int)lock_, m.owner_  );
#define MGDB_A(s) \
	fprintf(stderr, \
	"[%li][%i][%s][%i][MutexGuard::%s] lock[%x] thread[%li]\n", \
	GET_ULONG_THR_SELF(), getpid(), __FILE__, __LINE__, \
	s, (unsigned int)lock_, GET_ULONG_THR_SELF() );
#define CGDB(s) \
	fprintf( stderr, \
	"[%li][%i][%s][%i]Condition::%s] &c[%x] thread[%li]\n", \
	GET_ULONG_THR_SELF(), getpid(), __FILE__, __LINE__, \
	s, (unsigned int)&c, GET_ULONG_THR_SELF() );
#define CGDB_A(s) \
	fprintf( stderr, \
	"[%li][%i][%s][%i][Condition::%s] lock[%x] &c[%i] thread[%li]\n", \
	GET_ULONG_THR_SELF(), getpid(), __FILE__, __LINE__, \
	s, (unsigned int)m.lock_, (unsigned int)&c,  GET_ULONG_THR_SELF() );
#define CGDB_B(s) \
	fprintf( stderr, \
	"[%li][%i][%s][%i][Condition::%s] lock[%x] &c[%i] thread[%li]\n", \
	GET_ULONG_THR_SELF(), getpid(), __FILE__, __LINE__, \
	s, (unsigned int)m, (unsigned int)&c,  GET_ULONG_THR_SELF() );
#else
#define MDB(s)
#define MGDB(s)
#define MGDB_A(s) 
#define CGDB(s)
#define CGDB_A(s)
#define CGDB_B(s)
#endif

Condition::Condition() { 
	CGDB("Condition");
#ifdef FRAME_USES_ACE
    ACE_OS::cond_init( &c );
#else
	pthread_cond_init( &c, NULL ); 
#endif
};

Condition::~Condition() { 
	CGDB("~Condition");
#ifdef FRAME_USES_ACE
    ACE_OS::cond_destroy( &c );
#else
	pthread_cond_destroy( &c ); 
#endif
};

void 
Condition::wait( MutexGuard &m ){
	CGDB_A("wait");
#ifdef FRAME_USES_ACE
    ACE_OS::cond_wait( &c, m.lock_ );
#else
	pthread_cond_wait( &c, m.lock_ );
#endif
	CGDB_A("wait done");
};

void 
Condition::wait( frame_mutex_t *m){ 
	CGDB_B("wait( pthread_mutex_t *)");
#ifdef FRAME_USES_ACE
    ACE_OS::cond_wait( &c, m );
#else
	pthread_cond_wait( &c, m );
#endif
	CGDB_B("wait( pthread_mutex_t *) done");
};

bool 
Condition::timedwait( frame_mutex_t *m, long int seconds, long int nanoseconds ){ 
	CGDB_B("wait( pthread_mutex_t *)");
    struct timeval now;
    struct timespec timeout;
    gettimeofday( &now, NULL );
    timeout.tv_sec = now.tv_sec + seconds ;
    timeout.tv_nsec = now.tv_usec * 1000 ;

    int eintr_retry=10;
    while( eintr_retry-- )
    {
        int ret;
#ifdef FRAME_USES_ACE
        ACE_Time_Value atv(timeout);
        ret=ACE_OS::cond_timedwait( &c, m, &atv );
#else
        ret=pthread_cond_timedwait( &c, m, &timeout );
#endif
        if( ret == 0 )
        {
            return true;
        }
        if( ret == ETIMEDOUT )
        {
            return false;
        }
        if( ret != EINTR )
        {
            PANICV("Condition::timedwait result is not zero or ETIMEDOUT or EINTR");
        }
    }
    PANICV("Condition::timedwait exceeded EINTR retries.");
    //not reached.
    return false;
};

bool
Condition::timedwait( MutexGuard &m, long int seconds, long int nanoseconds  ){
	CGDB_A("wait");
	return timedwait( m.lock_, seconds, nanoseconds );
	CGDB_A("wait done");
};

void
Condition::signal(){ 
	CGDB("signal");
#ifdef FRAME_USES_ACE
    ACE_OS::cond_signal( &c );
#else
	pthread_cond_signal( &c ); 
#endif
	CGDB("signal done");
};
void
Condition::broadcast(){ 
	CGDB("broadcast");
#ifdef FRAME_USES_ACE
    ACE_OS::cond_broadcast( &c );
#else
	pthread_cond_broadcast( &c );
#endif
	CGDB("broadcast done");
};

Mutex::Mutex(){ 
#ifdef FRAME_USES_ACE
    ACE_OS::mutex_init( &lock_ );
#else
	pthread_mutex_init( &lock_, NULL ); 
#endif
#ifdef DEBUG
	owner_=0;
	MDB("ctor()");
#endif
};

Mutex::~Mutex(){ 
#ifdef FRAME_USES_ACE
    ACE_OS::mutex_destroy( &lock_ );
#else
	pthread_mutex_destroy( &lock_ ); 
#endif
#ifdef DEBUG
	owner_=0;
	MDB("dtor()");
#endif
};

void 
Mutex::lock(){ 
#ifdef DEBUG
	MDB("lock()");
#endif
#ifdef FRAME_USES_ACE
    ACE_OS::mutex_lock( &lock_ );
#else
	pthread_mutex_lock( &lock_ ); 
#endif
#ifdef DEBUG
	owner_=GET_ULONG_THR_SELF();
	MDB("successfully locked");
#endif
};
void 
Mutex::unlock(){ 
#ifdef DEBUG
	MDB("unlock()");
#endif
#ifdef FRAME_USES_ACE
    ACE_OS::mutex_unlock( &lock_ );
#else
	pthread_mutex_unlock( &lock_ ); 
#endif
#ifdef DEBUG
	owner_=0;
	MDB("successfully un locked");
#endif
};

const frame_mutex_t* Mutex::getMutex() const{
    return &lock_;
}

MutexGuard::MutexGuard( Mutex &m ):lock_(&m.lock_){
	MGDB("ctor Mutex &m");
#ifdef FRAME_USES_ACE
    ACE_OS::mutex_lock( lock_ );
#else
	pthread_mutex_lock( lock_ );
#endif
#ifdef DEBUG
	m.owner_=GET_ULONG_THR_SELF();
	MGDB("ctor Mutex &m succeeded");
#endif /* DEBUG */
};

MutexGuard::MutexGuard( frame_mutex_t *lock ):lock_(lock){
	MGDB_A( "ctor pthread_mutex_t *");
#ifdef FRAME_USES_ACE
    ACE_OS::mutex_lock( lock_ );
#else
	pthread_mutex_lock( lock_ );
#endif
	MGDB_A( "ctor pthread_mutex_t * succeeded");
};

MutexGuard::~MutexGuard(){ 
	MGDB_A( "MutexGuard::DTOR");
#ifdef FRAME_USES_ACE
    ACE_OS::mutex_unlock( lock_ );
#else
	pthread_mutex_unlock( lock_ ); 
#endif
	MGDB_A( "MutexGuard::DTOR suceeded");
};

frame_mutex_t* MutexGuard::getMutex() const{
    return lock_;
}
