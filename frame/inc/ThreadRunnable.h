
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ThreadRunnable.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 14:40:02 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ThreadRunnable.h,v 1.1 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef THREADRUNNABLE_H
#define THREADRUNNABLE_H 1

#include "AsyncRunnable.h"
    
class ThreadRunnablePriv ;
class ThreadRunnable : public AsyncRunnable
{
    friend class ThreadRunnablePriv ;
    public:
    virtual ~ThreadRunnable();
    ThreadRunnable(  RunnablePtr &work );

    virtual void start();
    virtual void run();
    virtual erunstate wait();

    private:
    ThreadRunnablePriv *d_;
    //not implemented
    ThreadRunnable();
    ThreadRunnable( const ThreadRunnable &rhs );
    ThreadRunnable &operator=( const ThreadRunnable &rhs );
};

#endif /* THREADRUNNABLE_H */
