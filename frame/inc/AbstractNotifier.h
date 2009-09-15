
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractNotifier.h
// Author:    mgrosso 
// Created:   Tue Aug 19 09:34:20 EDT 2003 on dhcp-172-18-100-101.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: AbstractNotifier.h,v 1.3 2005/01/24 18:23:59 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ABSTRACTNOTIFIER_H
#define ABSTRACTNOTIFIER_H 1

#include "Listener.h"
    
class AbstractNotifier : virtual public Listener
{
    public:
    virtual         ~AbstractNotifier();

    virtual void    notify()=0;
    //virtual void    notifyLast()=0;

    virtual void            registerListener( ListenerPtr &l )=0;
    virtual void            removeListener( ListenerPtr &l )=0;
    virtual void            registerListener( Listener *l )=0;
    virtual void            removeListener( Listener *l )=0;

    protected:
    //noop implementations for child compatibility
    AbstractNotifier();
    AbstractNotifier( const AbstractNotifier &rhs );
    AbstractNotifier &operator=( const AbstractNotifier &rhs );
};



#endif /* ABSTRACTNOTIFIER_H */
