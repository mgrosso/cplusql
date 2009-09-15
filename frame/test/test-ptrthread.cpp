#ident  "file_id $Id: test-ptrthread.cpp,v 1.1 2004/01/25 06:13:17 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      main.cpp
// Author:    Matt Grosso
// Created:   Wed Oct 14 20:42:59 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: test-ptrthread.cpp,v 1.1 2004/01/25 06:13:17 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include <stdio.h>
#include <vector>
#include <map>
#include <assert.h>
#include "Exclusion.h"
#include "Event.h"
#define DEBUG 1
#include "ptr.h"
#include "ptr_strdup.h"


class C {
	public:
		C():id_(0){ 
            int cnt = incrCounter();
			//fprintf(stderr,"C()%i class cntr=%i\n",id_, cnt );
            ++cnt;//avoid compiler warning.
		};
		C( int id):id_(id){ 
            int cnt = incrCounter();
			//fprintf(stderr,"C(int)%i class cntr=%i\n",id_, cnt );
            ++cnt;//avoid compiler warning.
		};
		virtual ~C(){ 
            int cnt = decrCounter();
			//fprintf(stderr,"~C()%i class cntr=%i\n",id_, cnt );
            if( cnt < 0 )
                fprintf(stderr,"ALERT ~C()");
		};
		C(const C &rhs){
			id_=rhs.id_; 
            int cnt = incrCounter();
            cnt;
			//fprintf(stderr,"C(const C &)%i class cntr=%i\n",id_, cnt );
		};
		C & operator=(const C &rhs){
			//fprintf(stderr,"C & operator=(const C &)%i rhs%i\n",id_,rhs.id_);
			id_=rhs.id_;
			return *this;
		};
        static int getCounter(){ 
            return cntr_;
        };
	protected:
        static int incrCounter(){ 
            MutexGuard m( cntr_lock_ );
            return ++cntr_;
        };
        static int decrCounter(){ 
            MutexGuard m( cntr_lock_ );
            return --cntr_;
        };
		int id_;
        static int cntr_;
        static Mutex cntr_lock_;
};
int C::cntr_=0;
Mutex C::cntr_lock_ ;

typedef struct threadtest_data
{
    threadtest_data()
    :pi(new int()), pc(new C())
    {
        ;//noop
    };
    ptr<int>    pi;
    ptr<C>      pc;
    int         inner_loop;
    int         outer_loop;
} threadtest_data_t;

class test_ptrthread_eventhandler : public EventHandler
{
    public:
    void handleEvent( void *arg )
    {
        threadtest_data_t *d=reinterpret_cast<threadtest_data_t *>(arg);
        int inner_loops=d->inner_loop;
        int outer_loops=d->outer_loop;

        for( int i=0; i< outer_loops; ++i )
        {
            ptr<int>    piarr [ inner_loops ]  ;
            ptr<C>      pcarr [ inner_loops ]  ;
            for( int j=0; j< inner_loops; ++j )
            {
                piarr[j]=d->pi;
                pcarr[j]=d->pc;
            }
        }
    };
};

void threadtest( int inner_loop, int outer_loop, int thread_count )
{
    assert( 0==C::getCounter());
    {
        ptr<threadtest_data_t > d (new threadtest_data_t());
        d->inner_loop=inner_loop;
        d->outer_loop=outer_loop;

        pEventHandler pe( new test_ptrthread_eventhandler());
        pEventFactory pf;
        EventQueue eq ( pf, pe, 0, thread_count, ULONG_MAX );

        for( int i=0; i<thread_count; ++i )
        {
            eq.push( reinterpret_cast<void *>(d.get()));
        }
        eq.gracefulShutdown();
    }
    assert( 0==C::getCounter());
};

int main( int argc, char **argv ){
	fprintf(stderr,"welcome to frame\n");
    fprintf(stderr,"beginning thread test\n");
    try{
        threadtest( 300, 1000, 30 );
	}catch(exception &be ){
		fprintf(stderr,"exception: %s\n", be.what());
        exit(1);
	}catch(... ){
		fprintf(stderr,"exception\n");
        exit(1);
	}
	fprintf(stderr,"bye\n");
	return 0;
};
