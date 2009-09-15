// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractJoint.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 21:15:04 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractJoint.h,v 1.4 2004/01/15 18:29:20 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACTJOINT_H
#define ABSTRACTJOINT_H 1

#define NEED_THREADS
#include "frame_config.h"
#include "ptr.h"
#include "ptr_strdup.h"
class AbstractJointPriv  ;

class AbstractJoint
{
    public:
    virtual ~AbstractJoint();

    //start()
    //
    //starts another thread to process relation into dest
    //start() may assume that all callers come from same thread.
    //
    void start();

    //wait()
    //
    //blocks current thread until other thread has processed relation
    //returns 0 if everything is ok. non-zero otherwise
    //
    int wait();

    //returns an error string or null depending on wait() outcome
    char_ptr error_string();

    //processes relation to dest in current thread. if unsuccessfull,
    //false will be returned, and error_string() will provide reason.
    virtual bool dowork()=0;

    private:
    AbstractJointPriv *d_;
    static frame_thread_return_t run( void *);
    static frame_thread_return_t errthread( void *);

    protected:
    void    setError( const char *s );
    void    setComplete();

    AbstractJoint();
    AbstractJoint( const AbstractJoint &rhs );
    AbstractJoint & operator=( const AbstractJoint &rhs );
};


typedef ptr< AbstractJoint > JointPtr ;

#endif /* ABSTRACTJOINT_H */
