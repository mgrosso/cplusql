#ident "file_id $Id: AbstractJoint.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Joint.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 21:24:09 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractJoint.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_THREADS
#include "frame_config.h"

#include "AbstractJoint.h"
#include "BaseException.h"
#include "Exclusion.h"
#include "Thread.h"
#include <stdlib.h>
#include <unistd.h>

#include "dbg.h"

class AbstractJointPriv
{
    private:
    friend class AbstractJoint;
    Thread     *thread_;
    bool        complete_;
    char_ptr    error_string_;
    Mutex       initonce_;

    static bool        err_thread_init_;
    static Thread *    err_thread_;
    static bool        err_flag_;
    static int         runcount_;
};

bool AbstractJointPriv::err_thread_init_ = false;
Thread *AbstractJointPriv::err_thread_ = NULL;
bool AbstractJointPriv::err_flag_ = false;
int  AbstractJointPriv::runcount_ = 0;

AbstractJoint::AbstractJoint()
{
    d_=new AbstractJointPriv();
    if(d_==NULL)
    {
        THROW_NOMEM;
    }
    d_->thread_=NULL;
    d_->complete_=false;
    d_->error_string_ = ptr_strdup( "" );
}

AbstractJoint::~AbstractJoint()
{
    if( d_->thread_ )
    {
        delete d_->thread_;
    }
    delete d_;
}

void
AbstractJoint::start()
{
    MutexGuard mg( d_->initonce_ );
    ++ AbstractJointPriv::runcount_;
    if( !AbstractJointPriv::err_thread_init_)
    {
        //should use pthread_once here...
        AbstractJointPriv::err_thread_init_ = true;
        AbstractJointPriv::err_thread_ = new Thread( AbstractJoint::errthread, NULL);
    }
    d_->thread_ = new Thread( AbstractJoint::run, (void *)this );
}

int
AbstractJoint::wait()
{
    d_->thread_->join();
    if( d_->complete_ )
    {
        return 0;
    }
    return -1;
}

void
AbstractJoint::setError( const char *s )
{
    d_->err_flag_=true;
    d_->error_string_ = ptr_strdup( s );
}

void
AbstractJoint::setComplete()
{
    d_->complete_=true;
}

char_ptr
AbstractJoint::error_string()
{
    return d_->error_string_;
}

frame_thread_return_t
AbstractJoint::run( void *j )
{
    AbstractJoint *jj = (AbstractJoint *)j;
    FDBG_SI( "AbstractJoint::run() start", jj);
    (void)jj->dowork();
    FDBG_SI( "AbstractJoint::run() stop", jj);
    return FRAME_THREAD_DEFAULT_RETURN;
}

frame_thread_return_t
AbstractJoint::errthread( void * )
{
    while( AbstractJointPriv::runcount_ && ! AbstractJointPriv::err_flag_ )
    {
#ifdef FRAME_USES_ACE
        ACE_OS::sleep(1);
#else
        sleep(1);
#endif
    }
    if( AbstractJointPriv::runcount_ )
    {
        //err_flag_ is true, and other joint threads may be hung.
        exit(1);
    }
    return FRAME_THREAD_DEFAULT_RETURN;
}

