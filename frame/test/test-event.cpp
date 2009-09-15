#ident  "file_id $Id: test-event.cpp,v 1.7 2004/01/20 06:11:48 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      emain.cpp
// Author:    Matt Grosso
// Created:   Wed Oct 21 23:13:45 EDT 1998 on circe
// Project:   
// Purpose:   unit test for Event.*
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: test-event.cpp,v 1.7 2004/01/20 06:11:48 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "dbg.h"
#include "BaseException.h"
#include "Exclusion.h"
#include "ptr.h"
#include "Event.h"
extern "C" {
#include <time.h>
#include <sys/time.h>
};
#include <limits.h>


class MyEvent {
public:
	MyEvent();
	~MyEvent();
	unsigned serial;
	unsigned f ;
	time_t start;
	time_t done;
	static int ser;
	static Mutex serlock;
    bool    hooked ;
};


int MyEvent::ser = 0;
Mutex MyEvent::serlock;

MyEvent::MyEvent(){
	start=time(NULL);
	done=0;
	MutexGuard mg( serlock );
	serial = ++ser; 
	f=ser + 1000 ;
    hooked = false;
	FDBG( "MyEvent", "Ctor",(unsigned long int)this );
};
MyEvent::~MyEvent(){
	FDBG( "MyEvent", "Dtor",(unsigned long int)this);
};

class MyEventHandler : public EventHandler{
	public:
	void handleEvent ( void *e );
    static int errors ;
    bool need_hook ;
};
int MyEventHandler::errors = 0;

class MyHookEventHandler : public EventHandler{
	public:
	void handleEvent ( void *e );
};


void 
MyHookEventHandler::handleEvent( void *e ){
	MyEvent *pme = (MyEvent *)e ;
    pme->hooked=true;
};

void 
MyEventHandler::handleEvent( void *e ){
	
	ptr<MyEvent> pme( (MyEvent *)e );
    if( pme->hooked != true && need_hook == true )
    {
        ++errors ;
		fprintf(stderr, "pme->hooked != true\n");
    }

	// a little io...
	char filename[50];
	memset(filename, '\0', 50 );
	snprintf( filename, 50, "/tmp/event-%x.txt", pme->serial );

    //fprintf(stderr, "Starting MyEventHandler[%li] %s\n", GET_ULONG_THR_SELF(), filename );

	FILE *tf =fopen( filename, "w+" );
	//fprintf(stderr, "tf fopen[%x]\n", (unsigned int)tf );
	if(!tf){
		perror(filename);
		return;
	}
	fprintf( tf, "welcome to test event %u, f=%u, t1=%lu, t2=?", 
		pme->serial, pme->f, pme->start );
	
	// now spin wheels...
	unsigned short b = (unsigned ) pme->f;
    for( int i=10; i; --i )
    {
        while( ++b )
        {
            ;//spin our wheels for a short time ...
        }
    }

	// now some more pointless io...
	for( unsigned down=pme->f; down>0; --down )
		fprintf( tf, "down to [%x]-----------------------\n", down );

	
	//fprintf(stderr, "tf fclose[%li]\n", (long int)tf );
	if(EOF==fclose( tf )){
		perror(filename);
		return;
	}
};


class MyEventFactory : public EventFactory {
public:
	MyEventFactory( int events ):events_(events),sofar_(0){
		//status( "CTOR" );
	}
	void * getNextEvent(){
		//status( "getNextEvent() top" );
		if(sofar_==0)
        {
			//fprintf(stderr, "Starting EventFactory[%li]\n", GET_ULONG_THR_SELF());
        }
		if( sofar_++ < events_ )
        {
			//status( "getNextEvent() sofar_++<events_, return new event" );
			return new MyEvent();
		}else
        {
			//fprintf(stderr,"Completed EventFactory[%li]\n", GET_ULONG_THR_SELF());
			throw EventFactoryFinished();
		}
		//status( "getNextEvent() sofar_++<events_, return new event" );
	};
	void status( const char *s ){
		fprintf(stderr, "MyEventFactory [%x][%s] sofar_[%x] events_[%x]\n",
			(unsigned int )this, s, sofar_, events_ );
	};
private:
	unsigned int events_;
	unsigned int sofar_;
};

typedef ptr<MyEventFactory> pMyEventFactory ;
typedef ptr<MyEventHandler> pMyEventHandler ;

void 
run_test( 
        int my_factory_events,
        int factories,
        int handlers,
        int maxq
        )
{
    struct timeval tv;
    struct timeval tv2;
    gettimeofday( &tv, NULL );
    fprintf(stderr,"run_test %i %i %i %i [%x %x ... ", 
        my_factory_events,
        factories,
        handlers,
        maxq,
        (unsigned int)tv.tv_sec,
        (unsigned int)tv.tv_usec
    );

    pEventFactory pmeft1 ( new MyEventFactory( my_factory_events )) ;
    MyEventHandler *pe = new MyEventHandler();
    pe->need_hook=true;
    pEventHandler peh1( pe );
    pEventHandler phook1( new MyHookEventHandler() );

    pEventFactory pmeft2 ( new MyEventFactory( my_factory_events )) ;
    pe = new MyEventHandler();
    pe->need_hook=false;
    pEventHandler peh2( pe );

    pEventFactory pmeft3 ( new MyEventFactory( my_factory_events )) ;
    pEventHandler phook3( new MyHookEventHandler() );
    pe = new MyEventHandler();
    pe->need_hook=true;
    pEventHandler peh3( pe );

    {
        EventQueue eq1( pmeft1, peh1, phook1, factories, handlers, maxq );
        EventQueue eq2( pmeft2, peh2, factories, handlers, maxq );
        EventQueue eq3( pmeft3, peh3, phook3, factories, handlers, maxq );
        eq1.wait();
        eq2.wait();
        eq3.wait();
    }

    gettimeofday( &tv2, NULL );
    time_t secs = tv2.tv_sec - tv.tv_sec ;
    time_t usecs = tv2.tv_usec - tv.tv_usec ;
    if( usecs < 0 )
    {
        usecs += 1000000 ;
        secs -= 1 ;
    }

    fprintf(stderr,"%x %x ] secs %li msecs %li \n", 
        (unsigned int)tv2.tv_sec,
        (unsigned int)tv2.tv_usec,
        secs,
        usecs
    );
    if( MyEventHandler::errors )
    {
        PANIC("MyEventHandler::errors");
    }
};

int main( int argc, char **argv )
{
    fprintf(stderr,"PTHREAD_THREADS_MAX=%i\n", PTHREAD_THREADS_MAX );
    try{

        //run_test( 10, 20, 20, INT_MAX );


        run_test( 1, 1, 1, 1 );
        run_test( 100, 2, 2, INT_MAX );
        run_test( 100, 1, 20, INT_MAX );
        run_test( 100, 5, 20, INT_MAX );
        run_test( 100, 5, 20, 20 );
        run_test( 100, 5, 20, 20 );
        run_test( 100, 5, 20, 20 );
        run_test( 100, 5, 20, 10 );
        run_test( 100, 5, 20, 1 );
        run_test( 1, 5, 20, 1 );

        run_test( 1, 5, 0, 1 );
        run_test( 1, 20, 0, 1 );
        run_test( 100, 5, 0, 1 );
        run_test( 100, 20, 0, 1 );

        return 0;
    }catch(exception &e ){
        fprintf(stderr,"exception caught in main(): %s\n", e.what());
    }catch( ... ){
        fprintf(stderr,"unknown exception caught in main()\n" );
    }
    return 1;
}

