
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AsyncRunnableCollection.h
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb  9 02:32:50 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: AsyncRunnableCollection.h,v 1.2 2004/09/20 21:29:51 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ASYNCRUNNABLECOLLECTION_H
#define ASYNCRUNNABLECOLLECTION_H 1

#include <list>
#include "Runnable.h"
#include "AsyncRunnable.h"

using namespace std;

typedef list<RunnablePtr>  runnable_list_t ;

class AsyncRunnableCollectionPriv ;
class AsyncRunnableCollection : public AsyncRunnable
{
    public:
                                AsyncRunnableCollection();
    virtual                     ~AsyncRunnableCollection();

    virtual void                setErrorExits( bool b );
    virtual bool                getErrorExits( );
    virtual void                add( AsyncRunnablePtr &r );
    virtual void                addThread( RunnablePtr &r );
    virtual list<RunnablePtr>   getRunnables();

    virtual erunstate           getRunState();
    virtual const char *        getErrorString();
    virtual void                start();
    virtual void                run();
    virtual erunstate           wait();

    protected:
    AsyncRunnableCollectionPriv *d_;
    private:
    //not implemented
    AsyncRunnableCollection( const AsyncRunnableCollection &rhs );
    AsyncRunnableCollection &operator=( const AsyncRunnableCollection &rhs );
};

typedef ptr< AsyncRunnableCollection > AsyncRunnableCollectionPtr ;


#endif /* ASYNCRUNNABLECOLLECTION_H */
