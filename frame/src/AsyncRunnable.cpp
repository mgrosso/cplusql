#ident "file_id $Id: AsyncRunnable.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AsyncRunnable.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 14:01:06 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: AsyncRunnable.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "AsyncRunnable.h"
#include "BaseException.h"

class AsyncRunnablePriv 
{
    private:
    friend class AsyncRunnable ;
    char        error_[BE_LEN];
    AsyncRunnable::erunstate    r_;
    AsyncRunnablePriv()
    :r_(AsyncRunnable::notrunning)
    {
        error_[0]='\0';
    };
};

AsyncRunnable::AsyncRunnable()
{
    d_=new AsyncRunnablePriv ();
}

AsyncRunnable::~AsyncRunnable()
{
    delete d_;
}

void
AsyncRunnable::setFailed( const char *error )
{
    d_->r_=failed;
    strncpy( d_->error_, error, BE_LEN-1 );
    d_->error_[BE_LEN-1]='\0';
}

void
AsyncRunnable::setRunState( erunstate r )
{
    d_->r_=r;
}

AsyncRunnable::erunstate
AsyncRunnable::getRunState()
{
    return d_->r_;
}

const char *
AsyncRunnable::getErrorString()
{
    return d_->error_;
}

