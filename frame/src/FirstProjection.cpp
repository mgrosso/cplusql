#ident "file_id $Id: FirstProjection.cpp,v 1.6 2004/09/21 23:47:32 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FirstProjection.cpp
// Author:    mgrosso 
// Created:   Tue Apr 15 06:59:42 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: FirstProjection.cpp,v 1.6 2004/09/21 23:47:32 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "FirstProjection.h"
#include "ptr_strdup.h"

    

class FirstProjectionPriv 
{
    private:
    friend class FirstProjection ;
    FirstProjectionPriv( ExpressionPtr &e )
    :src_(e),first_(true)
    {
        //noop
    };
    ExpressionPtr src_ ;
    bool first_;
};

FirstProjection::FirstProjection(ExpressionPtr &e)
{
    d_=new FirstProjectionPriv (e);
    set(ptr_strdup(""));
}

FirstProjection::~FirstProjection()
{
    delete d_;
}

void
FirstProjection::childNotify()
{
    if(d_->first_)
    {
        d_->src_->notify();
        set(d_->src_->getCharPtr());
        d_->first_=false;
    }
}

AbstractProjectionPtr
FirstProjection::copy()
{
    AbstractProjectionPtr ap( new FirstProjection( d_->src_ ));
    return ap;
}


