#ident "file_id $Id: ThreadRunnable.cpp,v 1.2 2004/08/11 21:42:44 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ThreadRunnable.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 14:40:12 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ThreadRunnable.cpp,v 1.2 2004/08/11 21:42:44 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ThreadRunnable.h"
#include "Exclusion.h"
#include "BaseException.h"
#include "Thread.h"

class ThreadRunnablePriv 
{
    private:
    friend class ThreadRunnable ;
    ThreadRunnable *            parent_;
    Thread *                    t_;
    RunnablePtr                 work_;
    Condition                   done_;
    Mutex                       lock_;

    ThreadRunnablePriv( ThreadRunnable *p, RunnablePtr &work )
    :parent_(p),t_(NULL),work_(work)
    {
        //noop
    };

    static frame_thread_return_t run( void *v );
};

frame_thread_return_t
ThreadRunnablePriv::run( void *v )
{
    ThreadRunnablePriv *self=reinterpret_cast<ThreadRunnablePriv *>(v);
    ThreadRunnable *p=self->parent_;
    try{
        self->work_->run();
        p->setRunState( AsyncRunnable::suceeded );
    }catch(exception &e){
        p->setFailed( e.what());
    }catch(...){
        p->setFailed( "ThreadRunnablePriv::run(): caught unknown exception.");
    }
    MutexGuard g(self->lock_);
    self->done_.broadcast();
    return FRAME_THREAD_DEFAULT_RETURN;
}



ThreadRunnable::ThreadRunnable( RunnablePtr &work )
{
    if( ! work.get())
    {
        PANICV("ThreadRunnable() work is NULL");
    }
    d_=new ThreadRunnablePriv ( this, work );
}

ThreadRunnable::~ThreadRunnable()
{
    //d_->t_->join();
    delete d_->t_;
    delete d_;
}

void
ThreadRunnable::start()
{
    MutexGuard g(d_->lock_);
    if( d_->t_ || running == getRunState())
    {
        PANICV( "ThreadRunnable::start(), allready started");
    }
    setRunState( running );
    d_->t_=new Thread( ThreadRunnablePriv::run, static_cast<void *>(d_) );
}

void
ThreadRunnable::run()
{
    ThreadRunnablePriv::run( static_cast<void *>(d_) );
}

AsyncRunnable::erunstate
ThreadRunnable::wait()
{
    MutexGuard g(d_->lock_);
    erunstate  s=getRunState();
    if( s==failed || s == suceeded )
    {
        return s;
    }
    d_->done_.wait( g );
    return getRunState();
}

