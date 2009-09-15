// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Thread.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Dec 28 17:12:49 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: Thread.h,v 1.3 2004/08/10 20:45:26 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef THREAD_H
#define THREAD_H 1

#define NEED_THREADS
#include "frame_config.h"

class ThreadPriv ;

class Thread
{
    public:

    Thread();
    //constructor starts the thread.
    Thread( frame_thread_function work, void *arg );

    //object dtor does not have any side affect on the os thread it represents.
    ~Thread();

	void init(frame_thread_function work, void *arg);

    //these calls act on other threads, requiring a Thread object.
    void join();//join() is void, for least common denominator portability
    void detach();
    void cancel();
    void test_cancel();

	bool isSelf() const;

    //some calls always happen on current thread and need no Thread object.
    static void exit(); // no exit value, for least common denominator portability
    static void set_cancel_type( int newtype, int *oldtype );
    static void set_cancel_state( int newstate, int *oldstate );

    private:
    ThreadPriv *d_;    

    //not implemented
    Thread( const Thread &rhs );
    Thread &operator=( const Thread &rhs );
};

#endif /* THREAD_H */
