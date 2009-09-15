
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      EventQueueExitRunnable.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Feb 20 17:57:45 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: EventQueueExitRunnable.h,v 1.1 2004/02/28 02:56:31 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef EVENTQUEUEEXITRUNNABLE_H
#define EVENTQUEUEEXITRUNNABLE_H 1
    
#include "Runnable.h" 
#include "Event.h" 

class EventQueueExitRunnablePriv ;

class EventQueueExitRunnable : public Runnable 
{
    public:
                                EventQueueExitRunnable( EventQueue *eq );
    virtual                     ~EventQueueExitRunnable();
    virtual void                run();

    private:
    EventQueue *eq_;

    //not implemented
    EventQueueExitRunnable();
    EventQueueExitRunnable( const EventQueueExitRunnable &rhs );
    EventQueueExitRunnable &operator=( const EventQueueExitRunnable &rhs );
};

#endif /* EVENTQUEUEEXITRUNNABLE_H */
