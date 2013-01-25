#ident "file_id $Id: AsyncRunnableCollection.cpp,v 1.3 2005/04/25 17:54:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AsyncRunnableCollection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb  9 02:32:50 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: AsyncRunnableCollection.cpp,v 1.3 2005/04/25 17:54:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdlib.h>
#include "AsyncRunnableCollectionPriv.h"
#include "AppLog.h"

//this is a private helper class which cooperates with AsyncRunnable
//by updating data in its Priv as its wrapped class completes.
class CollectionRunWrapper : public AsyncRunnable
{
    public:
    CollectionRunWrapper( 
        AsyncRunnableCollectionPriv *p, AsyncRunnablePtr &par, AsyncRunnable *ar );
    virtual ~CollectionRunWrapper();

    virtual erunstate           getRunState();
    virtual const char *        getErrorString();
    virtual void                start();
    virtual void                run();
    virtual erunstate           wait();

    private:
    AsyncRunnableCollectionPriv *p_;
    AsyncRunnablePtr par_ ;
    AsyncRunnable *ar_ ;
//not implemented
    CollectionRunWrapper();
};

AsyncRunnableCollectionPriv::AsyncRunnableCollectionPriv()
:error_exits_(true),t_(NULL),runcount_(0)
{
    ;
};

CollectionRunWrapper::CollectionRunWrapper(
    AsyncRunnableCollectionPriv *p, 
    AsyncRunnablePtr &par, 
    AsyncRunnable *ar 
        )
:p_(p),par_(par),ar_(ar)
{
    ;//noop
}

CollectionRunWrapper::~CollectionRunWrapper()
{
    ;//noop
}

void
CollectionRunWrapper::start()
{
    ar_->start();
}

void
CollectionRunWrapper::run()
{
    ar_->run();
    MutexGuard g( p_->lock_ );
    if( ! --p_->runcount_ )
    {
        p_->done_.broadcast();
    }
}

AsyncRunnable::erunstate
CollectionRunWrapper::wait()
{
    return ar_->wait();
}

AsyncRunnable::erunstate
CollectionRunWrapper::getRunState()
{
    return ar_->getRunState();
}

const char *
CollectionRunWrapper::getErrorString()
{
    return ar_->getErrorString();
}

AsyncRunnableCollection::AsyncRunnableCollection()
{
    d_=new AsyncRunnableCollectionPriv ();
}

AsyncRunnableCollection::~AsyncRunnableCollection()
{
    if( d_->t_ )
    {
        delete d_->t_;
    }
    delete d_;
}

void 
AsyncRunnableCollection::setErrorExits( bool b )
{
    d_->error_exits_=b;
}

bool 
AsyncRunnableCollection::getErrorExits( )
{
    return d_->error_exits_;
}

void 
AsyncRunnableCollection::add( AsyncRunnablePtr &r )
{
    AsyncRunnablePtr w=new CollectionRunWrapper( d_, r, r.get());
    d_->runlist_.push_back( w );
}

void 
AsyncRunnableCollection::addThread( RunnablePtr &r )
{
    AsyncRunnablePtr ar = new ThreadRunnable( r );
    add( ar );
    d_->runnables_.push_back( r );
}

runnable_list_t 
AsyncRunnableCollection::getRunnables()
{
    return d_->runnables_ ;
}

const char * 
AsyncRunnableCollection::getErrorString( )
{
    if( d_->t_ )
    {
        return d_->t_->getErrorString();
    }
    return "not running";
}

AsyncRunnable::erunstate
AsyncRunnableCollection::getRunState( )
{
    if( !d_->t_ )
    {
        return notrunning ;
    }
    return d_->t_->getRunState();
}

void        
AsyncRunnableCollection::start()
{
    MutexGuard g(d_->lock_);
    if( ! d_->t_ )
    {
        RunnablePtr rp = new ProxyRunnable( this );
        d_->t_ = new ThreadRunnable( rp );
        d_->t_->start();
    }
    else
    {
        PANICV( "AsyncRunnableCollection::start() allready started.");
    }
}

void        
AsyncRunnableCollection::run()
{
    unsigned int total, tfailed, trunning, tsuceeded;
    total = tfailed = trunning = tsuceeded = 0;
    unsigned errsleeps=3;
    runlist_t::iterator i;
    runlist_t::iterator e;
    char_ptr errbuf;
    for(
            i=d_->runlist_.begin(), e=d_->runlist_.end(); 
            i != e ;
            ++i
       )
    {
       (*i)->start();
       MutexGuard g(d_->lock_);
       ++d_->runcount_;
       ++total;
    }
    while(  (( tfailed + tsuceeded ) < total ) && 
            d_->runcount_ )
    {
        for(
                i=d_->runlist_.begin(), e=d_->runlist_.end(); 
                i != e ;
                ++i
           )
        {
           erunstate state = (*i)->getRunState(); 
           switch( state )
           {
                case failed: 
                    ++tfailed;
                    APPLOG_ABORT( "Runtime Error: %s", 
                        (*i)->getErrorString());
                    errbuf=errbuf+(*i)->getErrorString();
                    errbuf=errbuf+"\n";

                    break;
                case suceeded:
                    ++tsuceeded;
                    break;
                default:
                    break;
           }
           if( 
                   tfailed && 
                   (( tfailed + tsuceeded ) < total ) && 
                   ( ! --errsleeps ) &&
                   d_->error_exits_ 
           )
           {
               exit(1);
           }
           MutexGuard g(d_->lock_);
           d_->done_.timedwait( g, 1, 0 );
        }
    }
    if( tfailed )
    {
        PANICV("Runtime Errors: %s", errbuf.get());
    }
}

AsyncRunnable::erunstate   
AsyncRunnableCollection::wait()
{
    if( ! d_->t_ )
    {
        return notrunning ;
    }
    return d_->t_->wait();
}


