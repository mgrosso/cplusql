#ident "file_id $Id: SyncRunnableCollection.cpp,v 1.1 2004/09/20 22:34:20 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SyncRunnableCollection.cpp
// Author:    mgrosso 
// Created:   Thu Aug 26 18:10:45 PDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: SyncRunnableCollection.cpp,v 1.1 2004/09/20 22:34:20 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SyncRunnableCollection.h"
#include "AsyncRunnableCollectionPriv.h"
#include <string>
using namespace std;
    
class SyncRunnableCollectionPriv 
{
    private:
    friend class SyncRunnableCollection ;

    AsyncRunnableCollection::erunstate       state_ ;
    string                                  error_;
    SyncRunnableCollectionPriv()
    :state_( AsyncRunnableCollection::notrunning),
     error_("")
     {
         ;//noop
     };
};

SyncRunnableCollection::SyncRunnableCollection()
{
    dd_ = new SyncRunnableCollectionPriv();
}

SyncRunnableCollection::~SyncRunnableCollection()
{
    delete dd_ ;
}

SyncRunnableCollection::SyncRunnableCollection( runnable_list_t rlist )
{
    dd_ = new SyncRunnableCollectionPriv();
    runnable_list_t::iterator i = rlist.begin();
    runnable_list_t::iterator e = rlist.end();
    for( ; i != e ; ++i )
    {
        addThread( *i );
    }
    dd_->state_ = AsyncRunnableCollection::suceeded ;
}

void
SyncRunnableCollection::run()
{
    dd_->state_ = AsyncRunnableCollection::running ;
    runnable_list_t::iterator i = d_->runnables_.begin();
    runnable_list_t::iterator e = d_->runnables_.end();
    try {
        for( ; i != e ; ++i )
        {
            (*i)->run();
        }
    }catch( std::exception &e )
    {
        dd_->error_ = e.what();
        dd_->state_ = AsyncRunnableCollection::failed ;
        MutexGuard g( d_->lock_ );
        d_->done_.broadcast();
        throw ;
    }
    dd_->state_ = AsyncRunnableCollection::suceeded ;
    MutexGuard g( d_->lock_ );
    d_->done_.broadcast();
}

