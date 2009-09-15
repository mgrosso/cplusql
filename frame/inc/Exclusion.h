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
// $Id: Exclusion.h,v 1.6 2004/03/05 17:57:27 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MUTEXGUARD_H
#define MUTEXGUARD_H 1

#define NEED_THREADS
#include "frame_config.h"

class MutexGuard;
class Condition;

class Mutex {
	friend class MutexGuard;
	friend class Condition;
public:
	Mutex();
	~Mutex();
	void lock();
	void unlock();
    const frame_mutex_t* getMutex() const;
private:
	frame_mutex_t lock_ ;
#ifdef DEBUG
	unsigned long owner_ ;
#endif
};

class MutexGuard {
	friend class Condition;
public:
	MutexGuard( Mutex &m );
	MutexGuard( frame_mutex_t *lock );
    frame_mutex_t* getMutex() const;
	~MutexGuard();
private:
	frame_mutex_t *lock_ ;
};

class Condition {
public:
	Condition();
	~Condition();
	void wait( MutexGuard &g );
	void wait( frame_mutex_t *m );
	bool timedwait( MutexGuard &g, long int seconds, long int nanosecs );
	bool timedwait( frame_mutex_t *m, long int seconds, long int nanosecs );
	void signal();
	void broadcast();
private:
	frame_cond_t c;
};


#endif /* MUTEXGUARD_H */

