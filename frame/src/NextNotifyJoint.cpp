#ident "file_id $Id: NextNotifyJoint.cpp,v 1.7 2005/01/24 18:34:12 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NextNotifyJoint.cpp
// Author:    mgrosso 
// Created:   Mon Aug 11 09:55:12 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: NextNotifyJoint.cpp,v 1.7 2005/01/24 18:34:12 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_THREADS
#include "frame_config.h"
#include "NextNotifyJoint.h"
#include "BaseException.h"
#if 0
#define DEBUG 1
#endif
#include "dbg.h"

class NextNotifyJointPriv 
{
    private:
    friend class NextNotifyJoint ;
    AbstractDelimitedSourceNextPtr  source_;
    char_ptr                        name_;
    NextNotifyJointPriv( 
        AbstractDelimitedSourceNextPtr &source, 
        char_ptr &name
    )
    :source_(source), name_(name)
    {
        //noop
    };
};

NextNotifyJoint::NextNotifyJoint( 
    AbstractDelimitedSourceNextPtr &source, 
    char_ptr &name, 
    unsigned long skip, 
    unsigned long limit )
:Notifier(skip, limit )
{
    nnjp_=new NextNotifyJointPriv ( source, name );
}

NextNotifyJoint::~NextNotifyJoint()
{
    delete nnjp_;
}

const char *
NextNotifyJoint::getName()
{
    return nnjp_->name_.get();
}

void
NextNotifyJoint::run()
{
    FDBG_SI( "NextNotifyJoint::dowork() a", this );
    int i=0;
    bool success=false;
    BaseException enhanced("unknown");
    try {
        while( nnjp_->source_->next() && !finished())
        {
            notify();
            ++i;
            FDBG_SII( "NextNotifyJoint::dowork() b", this, i );
        }
        notifyLast();
        success=true;
        FDBG_SI( "NextNotifyJoint::dowork() c", this );
    }catch( exception & e )
    {
        enhanced = BaseException::factory( 
            "ABORT NextNotifyJoint::dowork() source relation name[%s] input line[%i] c++ error[%s]\n",
            nnjp_->name_.get(),
            nnjp_->source_->lineNum(),
            e.what()
        );
    }catch( ... )
    {
        enhanced = BaseException::factory( 
            "ABORT NextNotifyJoint::dowork() source relation name[%s] input line[%i] unknown error\n",
            nnjp_->name_.get(),
            nnjp_->source_->lineNum()
        );
    }
    if( !success )
    {
        throw enhanced;
    }
    FDBG_SI( "NextNotifyJoint::dowork() d", this );
}

