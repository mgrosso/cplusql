// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Event.h
// Author:    Matt Grosso
// Created:   Wed Oct 14 20:21:49 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Event.h,v 1.5 2004/01/25 02:32:54 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef EVENT_H
#define EVENT_H 1

#define NEED_THREADS
#include "frame_config.h"
#include "ptr.h"

class EventFactory 
{
public:
	virtual void *getNextEvent()=0;
	virtual ~EventFactory();
};
class EventHandler 
{
public:
	virtual void handleEvent( void * )=0;
	virtual ~EventHandler();
};
class EventFactoryFinished 
{ 
public:
	EventFactoryFinished();
};

typedef ptr< EventFactory > pEventFactory ;
typedef ptr< EventHandler > pEventHandler ;

class EventQueuePriv ;

class EventQueue 
{
    friend class EventQueuePriv ;
public:
	EventQueue( 
		pEventFactory &factory, 
        pEventHandler &handler, 
		unsigned long numFactories,  
        unsigned long numHandlers,
        unsigned long maxQueue
	);

	EventQueue( 
		pEventFactory &factory, 
        pEventHandler &handler, 
        pEventHandler &qhook, 
		unsigned long numFactories,  
        unsigned long numHandlers,
        unsigned long maxQueue
	);
	~EventQueue();
	void gracefulShutdown();
	void halt();
	void wait();
	int  size();
	void push( void *event );
	void handleNow( void *event );

    unsigned long getNumHandlers();
    unsigned long getNumFactories();

    //Feature wish list: 
    // get/setMaxQueue();
    // void addHandler(); or get/setHandlers();
	
private:
	//HELPERS
	static frame_thread_return_t handlerLoop( void *);
	static frame_thread_return_t factoryLoop( void *);
	
	//DATA
	EventQueuePriv *d_ ;
	
	// NOT IMPLEMENTED...
	EventQueue();
	EventQueue( const EventQueue & rhs );
	EventQueue& operator=( const EventQueue & rhs );
	
};
typedef ptr< EventQueue > EventQueuePtr ;

#endif /* EVENT_H 1 */
