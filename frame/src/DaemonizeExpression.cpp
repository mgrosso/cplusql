#ident "file_id $Id: DaemonizeExpression.cpp,v 1.1 2005/03/11 19:49:03 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DaemonizeExpression.cpp
// Author:    mgrosso 
// Created:   Sun Mar  6 20:55:41 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: DaemonizeExpression.cpp,v 1.1 2005/03/11 19:49:03 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "DaemonizeExpression.h"
#include "Daemon.h"
#include "ptr_strdup.h"

    

class DaemonizeExpressionPriv 
{
    private:
    bool        dofork_;
    char_ptr    dir_;
    char_ptr    pidfile_;
    friend class DaemonizeExpression ;
    DaemonizeExpressionPriv(
        bool dofork, const char *dir, const char *pidfile 
    )
    : dofork_(dofork), 
        dir_ (ptr_strdup_wnull(dir)), 
        pidfile_(ptr_strdup_wnull(pidfile ))
    {
        //noop
    };
};

DaemonizeExpression::DaemonizeExpression(
    bool dofork, const char *dir, const char *pidfile 
)
{
    d_=new DaemonizeExpressionPriv ( dofork, dir, pidfile );
}

DaemonizeExpression::~DaemonizeExpression()
{
    delete d_;
}


const char *
DaemonizeExpression::getRaw()
{
    return "";
}

const char *
DaemonizeExpression::getRaw() const
{
    return "";
}

void
DaemonizeExpression::notify() 
{
    Daemon::daemonize( d_->dofork_, d_->dir_.get(), d_->pidfile_.get());
}




