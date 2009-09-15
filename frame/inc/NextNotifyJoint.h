
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NextNotifyJoint.h
// Author:    mgrosso 
// Created:   Mon Aug 11 09:50:44 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: NextNotifyJoint.h,v 1.4 2005/01/24 18:34:12 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef NEXTNOTIFYJOINT_H
#define NEXTNOTIFYJOINT_H 1

#include "Notifier.h"
#include "AbstractDelimitedSourceNext.h"
#include "AbstractRelation.h"
#include "Runnable.h"
    
class NextNotifyJointPriv ;
class NextNotifyJoint : public Runnable, public Notifier
{
    public:
    NextNotifyJoint( 
        AbstractDelimitedSourceNextPtr &source, 
        char_ptr &name,
        unsigned long skip=0, 
        unsigned long limit=0 );

    virtual                 ~NextNotifyJoint();
    virtual void            run();    
    virtual const char *    getName();    

    private:
    NextNotifyJointPriv *nnjp_;

    //not implemented
    NextNotifyJoint();
    NextNotifyJoint( const NextNotifyJoint &rhs );
    NextNotifyJoint &operator=( const NextNotifyJoint &rhs );
};



#endif /* NEXTNOTIFYJOINT_H */
