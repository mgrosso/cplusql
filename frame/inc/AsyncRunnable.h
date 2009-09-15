
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AsyncRunnable.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 13:37:44 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: AsyncRunnable.h,v 1.1 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ASYNCRUNNABLE_H
#define ASYNCRUNNABLE_H 1

#include "ptr.h"
#include "Runnable.h"

class AsyncRunnablePriv ;

class AsyncRunnable : public Runnable
{
    public:
    enum erunstate { 
        notrunning=0, 
        running=1, 
        failed=2, 
        suceeded=3
    };

    //start()
    //
    //shouldnt hang. run() will begin happening in another thread or
    //process or whatever.
    //
    virtual void                start()=0;

    //run()
    //
    //does stuff in current thread. will hang until work is done.
    //
    virtual void                run()=0;

    //wait()
    //
    //blocks caller until run() is done.
    virtual erunstate            wait()=0;


    //somethings actually do get implemented here.
    //
    virtual erunstate           getRunState();
    virtual const char *        getErrorString();
    virtual                     ~AsyncRunnable();
    protected:
                                AsyncRunnable();
    virtual void                setFailed( const char *error );
    virtual void                setRunState( erunstate r );

    private:
    AsyncRunnablePriv *              d_;

    //not implemented
    AsyncRunnable( const AsyncRunnable &rhs );
    AsyncRunnable &operator=( const AsyncRunnable &rhs );
};

typedef ptr< AsyncRunnable > AsyncRunnablePtr ;

#endif /* ASYNCRUNNABLE_H */
