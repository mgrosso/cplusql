// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Listener.h
// Author:    djain Deepak Jain
// Created:   Wed Feb  5 16:13:58 PST 2003 on sf-devdw002.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: Listener.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LISTENER_H
#define LISTENER_H 1

#include "ptr.h"

class Listener
{
    public:
        virtual void notify() = 0;
        virtual void notifyLast(); //default noop implementation for convenience
        virtual ~Listener();

    protected:
        Listener();        

    private:
        Listener(const Listener &rhs);
        Listener &operator=(const Listener &rhs);
};
typedef ptr< Listener > ListenerPtr ;

#endif /* LISTENER_H */
