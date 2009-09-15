// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Notifier.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Mar 15 17:51:27 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: Notifier.h,v 1.5 2005/01/24 18:26:14 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef NOTIFIER_H
#define NOTIFIER_H 1

#include "AbstractNotifier.h"

class NotifierPriv ;

class Notifier : virtual public AbstractNotifier 
{
    public:
                    Notifier( unsigned long skip, unsigned long limit );
                    Notifier();
    virtual         ~Notifier();
    virtual void    notify();
    virtual void    notifyLast();
    virtual bool    finished();

    //extensions
    void            registerListener( ListenerPtr &l );
    void            removeListener( ListenerPtr &l );
    void            registerListener( Listener *l );
    void            removeListener( Listener *l );

    private:
    NotifierPriv *d_;

    //not implemented
    Notifier( const Notifier &rhs );
    Notifier & operator=( const Notifier &rhs );
};

#endif /* NOTIFIER_H */
