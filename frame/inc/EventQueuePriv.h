// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      EventQueuePriv.h
// Author:    Matt Grosso
// Created:   Wed Oct 14 20:52:03 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: EventQueuePriv.h,v 1.3 2004/01/15 18:29:20 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#error "this file is obsolete."

#ifndef EVENTQUEUEPRIV_H
#define EVENTQUEUEPRIV_H 1

#define NEED_THREADS
#include "frame_config.h"

#include <unistd.h>
#include <deque>

#include "Thread.h"
#include "BaseException.h"
#include "Exclusion.h"
#include "ptr.h"
#include "dbg.h"
#include "Event.h"
#include "assert.h"

#define EDBG( s, i ) FDBG( "EventQueue", s, (unsigned long int)i )
/* #define QEXIT( s, i ) EDBG( s, i ); pthread_exit(NULL); */
#define PUSH_FAILED -1
#define PUSH_OK 0

class EventQueuePriv {
	friend class EventQueue ;
private:
	EventQueuePriv( 
		pEventFactory factory, 
		pEventHandler handler, 
		unsigned long numFactories,
		unsigned long numHandlers,
		unsigned long maxQueue
	) :factory_(factory),handler_(handler), 
		numFactories_(numFactories), numHandlers_(numHandlers),maxQueue_(maxQueue),
        shutdown_(0),handlers_(NULL),factories_(NULL) 
    {
        // empty 
	};
    pEventFactory factory_ ;
    pEventHandler handler_ ;
    unsigned long numFactories_ ;
    unsigned long numHandlers_ ;
    unsigned long maxQueue_ ;
	int shutdown_ ;
	deque< void * > queue_ ;
	Mutex qlock_ ;
	Condition  event_ ;
	Condition  maxq_ ;
	Condition  done_ ;
	Thread *handlers_ ;
	Thread *factories_ ;
	int nofactory_ ;
	int push( void *event );
	Thread * initThreads( 
		unsigned int len, frame_thread_function loop, void *arg );
	void joinThreads( Thread *threads, int len );
	void cancelThreads( Thread *threads, int len );
};


#endif /* EVENTQUEUEPRIV_H */
