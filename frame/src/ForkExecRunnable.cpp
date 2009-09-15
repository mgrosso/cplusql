#ident "file_id $Id: ForkExecRunnable.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ForkExecRunnable.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 16:33:12 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. 
// 
// $Id: ForkExecRunnable.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ForkExecRunnable.h"
#include "ptr_strdup.h"
#include "Exclusion.h"
#include "ExecWrap.h"
#include "BaseException.h"

class ForkExecRunnablePriv 
{
    private:
    friend class ForkExecRunnable ;
    char_ptr    command_;
    char_ptr    infile_;
    char_ptr    outfile_;
    char_ptr    errfile_;
    pid_t       pid_;
    int         exit_status_;
    Mutex       lock_;
    Condition   started_ ;
    Condition   done_ ;
    bool        waiting_;
    ForkExecRunnablePriv(
        const char *command,
        const char *infile,
        const char *outfile,
        const char *errfile
    )
    :command_(ptr_strdup(command)),
    infile_(ptr_strdup_wnull(infile)),
    outfile_(ptr_strdup_wnull(outfile)),
    errfile_(ptr_strdup_wnull(errfile)),
    pid_(0),
    exit_status_(-1)
    {
        //noop
    };
};

ForkExecRunnable::ForkExecRunnable(
        const char *command,
        const char *infile,
        const char *outfile,
        const char *errfile
)
{
    if( ! command || ! *command )
    {
        PANICV( "ForkExecRunnable(): cant exec null command.");
    }
    d_=new ForkExecRunnablePriv ( command, infile, outfile, errfile);
}

ForkExecRunnable::~ForkExecRunnable()
{
    ExecWrap::done_watching(d_->pid_);
    delete d_;
}

void
ForkExecRunnable::start()
{
    MutexGuard g( d_->lock_ );
    d_->pid_=ExecWrap::fork_exec_watch(
            d_->command_.get(),
            NULL, NULL, NULL,
            d_->infile_.get(),
            d_->outfile_.get(),
            d_->infile_.get()
    );
    d_->started_.broadcast();
}

void
ForkExecRunnable::run()
{
    start();
    wait();
}

AsyncRunnable::erunstate
ForkExecRunnable::wait()
{
    bool dowait=false;
    {
        MutexGuard g( d_->lock_ );
        if( d_->pid_ < 1 )
        {
            d_->started_.wait(g);
        }

        //d_->pid_ is gaurantied to have a value at this point.
        if( ! d_->waiting_ )
        {
            dowait=true;
            d_->waiting_=true;
        }
    }
    if( dowait )
    {
        d_->exit_status_ = ExecWrap::wait_for_exit_status(d_->pid_);
        MutexGuard g( d_->lock_ );
        d_->done_.broadcast();
    }
    else
    {
        MutexGuard g( d_->lock_ );
        d_->done_.wait(g);
    }
    return getRunState();
}

AsyncRunnable::erunstate
ForkExecRunnable::getRunState()
{
    if( d_->pid_ < 1 )
    {
        return notrunning;
    }
    int s;
    if( ! d_->waiting_ && d_->exit_status_ == -1 )
    {
        s=ExecWrap::check_for_exit_status(d_->pid_);
        if( s > -1 )
        {
            d_->exit_status_=ExecWrap::wait_for_exit_status(d_->pid_);
        }
    } 
    //at this point, either exit_status is -1 but someone is blocking for
    //the result, or we have updated the exit_status_ with nonblocking 
    //calls, or the process exited so that exit_status was not -1.
    if( d_->exit_status_ == -1  )
    {
        return running;
    }
    else if( d_->exit_status_  == 0 )
    {
        return suceeded ;
    }
    else
    {
        return failed;
    }
    return d_->exit_status_  ? failed : suceeded ;
}

