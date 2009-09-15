
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyEventHandler.h
// Author:    mgrosso Matthew Grosso
// Created:   Wed Jan 21 20:12:14 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ProxyEventHandler.h,v 1.1 2004/01/25 02:24:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PROXYEVENTHANDLER_H
#define PROXYEVENTHANDLER_H 1
#include "Event.h"

class ProxyEventHandler : public EventHandler
{
    public:
    ProxyEventHandler( EventHandler *h );
    virtual ~ProxyEventHandler();
    virtual void handleEvent( void *e );

    private:
    EventHandler *h ;
    //not implemented
    ProxyEventHandler();
    ProxyEventHandler( const ProxyEventHandler &rhs );
    ProxyEventHandler &operator=( const ProxyEventHandler &rhs );
};

#endif /* PROXYEVENTHANDLER_H */
