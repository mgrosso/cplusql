
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ForkExecRunnable.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 16:33:12 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. 
// 
// $Id: ForkExecRunnable.h,v 1.1 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef FORKEXECRUNNABLE_H
#define FORKEXECRUNNABLE_H 1
    
#include "AsyncRunnable.h"

class ForkExecRunnablePriv ;
class ForkExecRunnable : public AsyncRunnable
{
    public:
    ForkExecRunnable(
        const char *command,
        const char *infile,
        const char *outfile,
        const char *errfile
    );
    virtual                     ~ForkExecRunnable();
    virtual void                start();
    virtual void                run();
    virtual erunstate           wait();
    virtual erunstate           getRunState();

    private:
    ForkExecRunnablePriv *d_;
    //not implemented
    ForkExecRunnable();
    ForkExecRunnable( const ForkExecRunnable &rhs );
    ForkExecRunnable &operator=( const ForkExecRunnable &rhs );
};

#endif /* FORKEXECRUNNABLE_H */
