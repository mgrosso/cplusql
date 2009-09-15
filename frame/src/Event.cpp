#ident "file_id $Id: Event.cpp,v 1.12 2004/08/11 21:42:43 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Event.cpp
// Author:    Matt Grosso
// Created:   Wed Oct 14 22:06:50 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Event.cpp,v 1.12 2004/08/11 21:42:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#define NEED_THREADS
#include "frame_config.h"

#include <unistd.h>
#include <deque>
#include <exception>

#include "Thread.h"
#include "BaseException.h"
#include "Exclusion.h"
#include "ptr.h"
#include "dbg.h"
#include "Event.h"
#include "assert.h"

#define EDBG( s, i ) FDBG( "EventQueue", s, (unsigned long int)i )
#define EVDBG(s) EDBG(s,10);
/* #define QEXIT( s, i ) EDBG( s, i ); pthread_exit(NULL); */
#define PUSH_FAILED -1
#define PUSH_OK 0

class EventQueuePriv {
	friend class EventQueue ;
private:
    EventQueuePriv(
        pEventFactory &factory, 
        pEventHandler &handler, 
        pEventHandler &qhook, 
        unsigned long numFactories,
        unsigned long numHandlers,
        unsigned long maxQueue
    );
	virtual ~EventQueuePriv();

    //member helpers
	int push( void *event );
	void handleNow( void *e );

    //static helpers
	static Thread * initThreads( 
            unsigned int len, frame_thread_function loop, void *arg );
	static void joinThreads( Thread *threads, int len );
	static void cancelThreads( Thread *threads, int len );
	static void handleEvent( pEventHandler &h, void *e );

    pEventFactory factory_ ;
    pEventHandler handler_ ;
    pEventHandler qhook_ ;
    unsigned long startFactories_ ;
    unsigned long startHandlers_ ;
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
	int have_hook_ ;
	int have_handler_ ;

};

EventHandler::~EventHandler()
{
    ;//noop
};

EventFactory::~EventFactory()
{
    ;//noop
};

// ----------------------------------------------------------------------
// a few helper functions
// ----------------------------------------------------------------------
Thread * 
EventQueuePriv::initThreads( 
	unsigned int len, frame_thread_function loop, void *arg ){

	//EVDBG("init threads");
    if( ! len )
    {
        return NULL ;
    }
	Thread *threads= new Thread[ len ];
	for (unsigned int i = 0; i < len; i++)
	{
		threads[i].init(loop, arg);
	}

	//EVDBG("init threads done");
	return threads;
};

void 
EventQueuePriv::joinThreads( Thread *threads, int len ){
	//EVDBG("join threads");
	for( int i=0; i<len ; i++ )
    {
        //errors ignored, since all possible errors imply that 
        //threads have allready been cleaned up.
        try{
            (void)  threads[i].join();
        }catch( BaseException &be )
        {
            ;//ignore.
        }
    }
	//EVDBG("join threads done");
};

void 
EventQueuePriv::cancelThreads( Thread *threads, int len ){
	//EVDBG("cancel threads");
	for( int i=0; i<len ; i++ )
    {
		(void)  threads[i].cancel();
    }
	//EVDBG("cancel threads done");
};


// ----------------------------------------------------------------------
// EventQueue function definitions
// ----------------------------------------------------------------------

EventQueue::~EventQueue(){
	//EVDBG("~EventQueue");
	gracefulShutdown();
	if (d_ != (EventQueuePriv *) 0)
	{
		delete d_;
	}
	//EVDBG("~EventQueue done");
};

void 
EventQueue::wait()
{
    while( d_->numFactories_ || d_->numHandlers_ )
    {
        MutexGuard mg(d_->qlock_);
        if( d_->numFactories_ || d_->numHandlers_ )
        {
            d_->done_.wait( mg );
        }
    }
    EventQueuePriv::joinThreads( d_->factories_, d_->startFactories_ );
    EventQueuePriv::joinThreads( d_->handlers_, d_->startHandlers_ );
};

int 
EventQueue::size()
{
	return d_->queue_.size();
};


void 
EventQueue::gracefulShutdown()
{
	if(d_->shutdown_)
    {
		return;
    }
	//EVDBG("graceful shutdown");
	{
		MutexGuard mg(d_->qlock_);
		d_->shutdown_=1;
		d_->maxq_.broadcast();
		d_->event_.broadcast();
	}
    EventQueuePriv::joinThreads( d_->factories_, d_->startFactories_ );
    EventQueuePriv::joinThreads( d_->handlers_, d_->startHandlers_ );
    MutexGuard mg(d_->qlock_);
    d_->done_.broadcast();
	//EVDBG("graceful shutdown done");
};


void 
EventQueue::halt(){
	//EVDBG("halt shutdown");
	d_->shutdown_=1;
    EventQueuePriv::cancelThreads( d_->factories_, d_->numFactories_ );
    EventQueuePriv::cancelThreads( d_->handlers_, d_->numHandlers_ );
	//EVDBG("halt shutdown done");
};


EventFactoryFinished::EventFactoryFinished(){};

// ----------------------------------------------------------------------
// Event handler and factory loops.
// ----------------------------------------------------------------------

void 
EventQueue::push( void *event )
{
	d_->push(event);
};	

void 
EventQueue::handleNow( void *event )
{
    d_->handleNow( event );
}

void
EventQueuePriv::handleNow( void *event )
{
    if( have_hook_ )
    {
        MutexGuard mgrd( qlock_ );
        handleEvent( qhook_, event );
    }
    if( have_handler_ )
    {
        handleEvent( handler_, event );
    }
}

int 
EventQueuePriv::push( void *event )
{
    if( numHandlers_ == 0 )
    {
        if(shutdown_)
        {
            MutexGuard mg( qlock_ );
            event_.broadcast();
            return PUSH_FAILED ;
        }
        handleNow( event );
        return PUSH_OK;
    }
	if(shutdown_)
    {
		return PUSH_FAILED ;
    }
	MutexGuard mg( qlock_ );
	if(shutdown_)
    {
		return PUSH_FAILED ;
    }
    while(queue_.size()>=maxQueue_ && ! shutdown_ )
    {
        maxq_.wait(mg);
    }
	if(shutdown_)
    {
		return PUSH_FAILED ;
    }
	queue_.push_back( event );
	event_.signal();
	return PUSH_OK ;
};

frame_thread_return_t
EventQueue::handlerLoop( void *eventq )
{
	
	EventQueuePriv *p=(EventQueuePriv *)eventq;
    Thread::set_cancel_type( THR_CANCEL_ASYNCHRONOUS, NULL );
    
	while(1){
		void * e=NULL;
		{//scope mutexguard
			MutexGuard mg( p->qlock_ );
			if(p->shutdown_ && p->queue_.size() == 0){
				//EDBG("handlerLoop break",p->queue_.size());
				break;
			}
			// only wait if there is still someone producing events
			// and there is not allready events waiting for us in the 
			// queue.
			if( 
				((p->numFactories_!=0)||(p->nofactory_)) && 
				( p->queue_.size() == 0 )
			){
				p->event_.wait( mg );
            }
			if(p->shutdown_ && p->queue_.size() == 0)
            {
				break;
			}
			if((0==p->nofactory_) && (0==p->numFactories_) && 
                (0==p->queue_.size()))
            {
				p->done_.broadcast();
				break;
			}
			if( 0==p->queue_.size())
            {
				continue;
            }
			e= p->queue_.front();
            if( p->have_hook_ )
            {
                EventQueuePriv::handleEvent( p->qhook_, e );
            }
			p->queue_.pop_front();
            //ALWAYS TRUE! if(p->queue_.size()>=p->maxQueue_)
            p->maxq_.signal();
			//EDBG("handlerLoop just popped Q",p->queue_.size());
		}
        //e will only be NULL if NULL is what was in the queue from 
        //front().  no other control flow leads here. We do not worry 
        //if e==NULL; we trust that is what the user meant to do.
        if( p->have_handler_ )
        {
            EventQueuePriv::handleEvent( p->handler_, e );
        }
	}
    MutexGuard mg( p->qlock_ );
    --p->numHandlers_ ;
    if( 0==p->numHandlers_ && 0==p->numFactories_ )
    {
        p->done_.broadcast();
    }
    else if( 0!=p->numHandlers_ )
    {
        p->event_.broadcast();
    }
    return FRAME_THREAD_DEFAULT_RETURN;
};

frame_thread_return_t
EventQueue::factoryLoop( void *eventq ){
	
	EventQueuePriv *p=(EventQueuePriv *)eventq;
    Thread::set_cancel_type( THR_CANCEL_ASYNCHRONOUS, NULL );
	
	//EVDBG("factoryLoop");
	while(!p->shutdown_){
		try{
			//EVDBG("factory Loop about to getNextEvent");
			void * e = p->factory_->getNextEvent();
			//EVDBG("got new void *");
			if( PUSH_FAILED==p->push( e ))
				break;
		}catch( EventFactoryFinished &eff ){
			break;
		}catch( BaseException &be ){
			fprintf(stderr, "Exception in EventQueue::factoryLoop:[%s]\n",
				be.getMessage() );
		}catch( ... ){
			fprintf(stderr,"Exception in EventQueue::factoryLoop[Unknown]\n");
		}
	}
    MutexGuard mg( p->qlock_ );
    --p->numFactories_ ;
    if( 0== p->numFactories_ && 0==p->numHandlers_ )
    {
        p->done_.broadcast();
    }
    else if( 0!=p->numHandlers_ )
    {
        p->event_.broadcast();
    }
    return FRAME_THREAD_DEFAULT_RETURN ;
};

EventQueue::EventQueue( 
	pEventFactory &factory, 
	pEventHandler &handler, 
	pEventHandler &qhook, 
	unsigned long numFactories,
	unsigned long numHandlers,
    unsigned long maxQueue )
{
	d_=new EventQueuePriv( 
        factory, handler, qhook, numFactories, numHandlers, maxQueue);
};

EventQueue::EventQueue( 
	pEventFactory &factory, 
	pEventHandler &handler, 
	unsigned long numFactories,
	unsigned long numHandlers,
    unsigned long maxQueue )
{
    pEventHandler qhook(NULL);
	d_=new EventQueuePriv( 
        factory, handler, qhook, numFactories, numHandlers, maxQueue);
};

EventQueuePriv::EventQueuePriv( 
    pEventFactory &factory, 
    pEventHandler &handler, 
    pEventHandler &qhook, 
    unsigned long numFactories,
    unsigned long numHandlers,
    unsigned long maxQueue
)
    :factory_(factory),handler_(handler), qhook_(qhook),
    startFactories_(numFactories), startHandlers_(numHandlers),
    numFactories_(numFactories), numHandlers_(numHandlers),maxQueue_(maxQueue),
    shutdown_(0),handlers_(NULL),factories_(NULL) 
{
    if(! handler_.get() && !qhook_.get() && numHandlers_ > 0 )
    {
        PANIC( "EventQueue::ctor specified null handler and hook functions, and non-zero number of handlers, why?" );
    }
    have_hook_=false;
    if( NULL!=qhook_.get())
    {
        have_hook_=true;
    }
    have_handler_=false;
    if( NULL!=handler_.get())
    {
        have_handler_=true;
    }

    (numFactories_ == 0 ) ?  nofactory_=1 : nofactory_=0 ;

    handlers_ = EventQueuePriv::initThreads( 
        numHandlers_, EventQueue::handlerLoop, this );
    factories_ = EventQueuePriv::initThreads( 
        numFactories_, EventQueue::factoryLoop, this );
}


EventQueuePriv::~EventQueuePriv()
{
	if (handlers_ != (Thread *) 0)
	{
		for (unsigned long i = 0; i < numHandlers_; i++)
		{
			if (!handlers_[i].isSelf())
			{
				handlers_[i].cancel();
				handlers_[i].join();
			}
		}
		delete [] handlers_;
		handlers_ = (Thread *) 0;
	}
	if (factories_ != (Thread *) 0)
	{
		for (unsigned long i = 0; i < numFactories_; i++)
		{
			if (!factories_[i].isSelf())
			{
				factories_[i].cancel();
				factories_[i].join();
			}
		}
		delete [] factories_;
		factories_ = (Thread *) 0;
	}
}


void 
EventQueuePriv::handleEvent( pEventHandler &h, void *e )
{
    try{
        h->handleEvent( e );
    }catch( exception &ex )
    {
        fprintf(stderr, 
            "Exception in EventQueue::handlerLoop:[%s]\n",
            ex.what() 
        );
    }catch( ... )
    {
        fprintf(stderr,
            "Exception in EventQueue::handlerLoop[Unknown]\n"
        );
    }
}

unsigned long 
EventQueue::getNumHandlers()
{
    return d_->startHandlers_ ;
}

unsigned long 
EventQueue::getNumFactories()
{
    return d_->startFactories_ ;
}

