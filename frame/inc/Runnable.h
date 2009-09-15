
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Runnable.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 14:33:09 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: Runnable.h,v 1.1 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef RUNNABLE_H
#define RUNNABLE_H 1

#include "ptr.h"

class Runnable
{
    public:
    virtual ~Runnable();
    virtual void    run()=0;

    private:
};
typedef ptr< Runnable > RunnablePtr ;

#endif /* RUNNABLE_H */
