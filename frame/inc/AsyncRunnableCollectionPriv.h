// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AsyncRunnableCollectionPriv.h
// Author:    mgrosso 
// Created:   Thu Aug 26 19:44:51 PDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: AsyncRunnableCollectionPriv.h,v 1.1 2004/09/20 21:29:51 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ASYNCRUNNABLECOLLECTIONPRIV_H
#define ASYNCRUNNABLECOLLECTIONPRIV_H 1
    
#include "frame_config.h"
#include "ptr.h"
using namespace std;
#include <list>

#include "AsyncRunnableCollection.h"
#include "Exclusion.h"
#include "ThreadRunnable.h"
#include "ProxyRunnable.h"
#include "BaseException.h"
#include "ptr_strdup.h"

typedef list< AsyncRunnablePtr >  runlist_t;

class AsyncRunnableCollectionPriv 
{
    public:
    friend class CollectionRunWrapper ;
    friend class AsyncRunnableCollection ;
    runlist_t       runlist_;
    runnable_list_t runnables_ ;
    Mutex           lock_;
    Condition       done_;
    bool            error_exits_;
    ThreadRunnable *t_;
    int             runcount_;

    AsyncRunnableCollectionPriv();
};

#endif /* ASYNCRUNNABLECOLLECTIONPRIV_H */
