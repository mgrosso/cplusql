#ident "file_id $Id: Thread.cpp,v 1.4 2004/08/10 20:48:19 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Thread.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Tue Dec 30 00:22:45 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: Thread.cpp,v 1.4 2004/08/10 20:48:19 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_THREADS
#include "frame_config.h"
#include "Thread.h"
#include "BaseException.h"
#include "Exclusion.h"

class ThreadPriv 
{
    private:
    friend class Thread ;
    frame_thread_t          thread_;
    Mutex                   join_lock_; 
    bool                    joined_; 
    ThreadPriv( frame_thread_function work, void *arg )
    :joined_(false)
    {
        int ret;
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
        ret=ACE_OS::thr_create( work, arg, 0, &thread_ );
#else
        ret=pthread_create( &thread_, 0, work, arg );
#endif
        if(ret!=0)
        {
            PANIC( "unable to create thread." );
        }
    };
};

Thread::Thread()
{
	d_ = (ThreadPriv *) 0;
}


Thread::Thread( frame_thread_function work, void *arg )
{
	d_ = (ThreadPriv *) 0;
	init(work, arg);
}

Thread::~Thread()
{
	if (d_ != (ThreadPriv *) 0)
	{
		delete d_;
		d_ = (ThreadPriv *) 0;
	}
}

void
Thread::init(frame_thread_function work, void *arg )
{
	if (d_ == (ThreadPriv *) 0)
	{
		d_ = new ThreadPriv ( work, arg );
	}
}

void
Thread::join()
{
    int ret;
    frame_thread_return_t   gb;
    MutexGuard jlock( d_->join_lock_ );
    if( ! d_->joined_ )
    {
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
        ret=ACE_OS::thr_join( ACE_OS::thr_self(), &d_->thread_, &gb );
#else
        ret=pthread_join( d_->thread_, &gb );
#endif
        if(ret!=0)
        {
            PANIC_I( "unable to join thread.", ret);
        }
        d_->joined_ = true ;
    }
}

void
Thread::detach()
{
    int ret;
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
    ret=0;//noop
#else
    ret=pthread_detach( d_->thread_ );
#endif
    if(ret!=0 && ret != EINVAL )
    {
        PANIC( "unable to detach thread, does it still exist?" );
    }
}

void
Thread::cancel()
{
    int ret;
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
    ret=ACE_OS::thr_cancel( d_->thread_ );
#else
    ret=pthread_cancel( d_->thread_ );
#endif
    if(ret!=0 )
    {
        PANIC( "unable to cancel thread." );
    }
}

void
Thread::test_cancel()
{
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
    ACE_OS::thr_testcancel();
#else
    pthread_testcancel();
#endif
}

bool
Thread::isSelf() const
{
	bool retVal = false;

#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
	retVal = ACE_OS::thr_equal(ACE_OS::thr_self(), d_->thread_);
#else
	retVal = pthread_equal(pthread_self(), d_->thread_);
#endif

	return retVal;
}


void
Thread::exit()
{
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
    ACE_OS::thr_exit( FRAME_THREAD_DEFAULT_RETURN );
#else
    pthread_exit( FRAME_THREAD_DEFAULT_RETURN );
#endif
}

void
Thread::set_cancel_type( int newtype, int *oldtype )
{
    int ret;
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
    ret=ACE_OS::thr_setcanceltype( newtype, oldtype );
#else
    ret=pthread_setcanceltype( newtype, oldtype );
#endif
    if(ret!=0 )
    {
        PANIC( "unable to set cancel type." );
    }
}

void
Thread::set_cancel_state( int newtype, int *oldtype )
{
    int ret;
#if defined (FRAME_USES_ACE) && ! defined(HAVE_PTHREAD_H)
    ret=ACE_OS::thr_setcancelstate( newtype, oldtype );
#else
    ret=pthread_setcancelstate( newtype, oldtype );
#endif
    if(ret!=0 )
    {
        PANIC( "unable to set cancel state." );
    }
}

