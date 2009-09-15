
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SyncRunnableCollection.h
// Author:    mgrosso 
// Created:   Thu Aug 26 18:10:45 PDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: SyncRunnableCollection.h,v 1.1 2004/09/20 22:34:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SYNCRUNNABLECOLLECTION_H
#define SYNCRUNNABLECOLLECTION_H 1

#include "AsyncRunnableCollection.h"

class SyncRunnableCollectionPriv ;
class SyncRunnableCollection : public AsyncRunnableCollection
{
public:
    SyncRunnableCollection();
    SyncRunnableCollection( runnable_list_t rlist );
    virtual void run();
    virtual ~SyncRunnableCollection();

private:
    SyncRunnableCollectionPriv *dd_ ;
    //not implemented
    SyncRunnableCollection( const SyncRunnableCollection &rhs );
    SyncRunnableCollection &operator=( const SyncRunnableCollection &rhs );
};

#endif /* SYNCRUNNABLECOLLECTION_H */
