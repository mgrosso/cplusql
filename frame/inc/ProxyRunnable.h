
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyRunnable.h
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb  9 02:54:43 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ProxyRunnable.h,v 1.1 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PROXYRUNNABLE_H
#define PROXYRUNNABLE_H 1
#include "Runnable.h"

class ProxyRunnable : public Runnable
{
    public:
    ProxyRunnable( Runnable *r );
    virtual ~ProxyRunnable();
    virtual void run();

    private:
    Runnable *r_ ;

    //not implemented
    ProxyRunnable();
    ProxyRunnable( const ProxyRunnable &rhs );
    ProxyRunnable &operator=( const ProxyRunnable &rhs );
};

#endif /* PROXYRUNNABLE_H */
