#ident "file_id $Id: LastProjection.cpp,v 1.6 2004/09/21 23:48:16 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LastProjection.cpp
// Author:    mgrosso 
// Created:   Tue Apr 15 06:59:42 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: LastProjection.cpp,v 1.6 2004/09/21 23:48:16 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "LastProjection.h"
#include "ptr_strdup.h"

    

class LastProjectionPriv 
{
    private:
    friend class LastProjection ;
    LastProjectionPriv( ExpressionPtr &e )
    :src_(e)
    {
        //noop
    };
    ExpressionPtr src_ ;
    char_ptr val_;
};

LastProjection::LastProjection(ExpressionPtr &e)
{
    d_=new LastProjectionPriv (e);
    set(ptr_strdup(""));
}

LastProjection::~LastProjection()
{
    delete d_;
}

void
LastProjection::childNotify()
{
    d_->src_->notify();
    set(d_->src_->getCharPtr());
}

AbstractProjectionPtr
LastProjection::copy()
{
    AbstractProjectionPtr ap( new LastProjection( d_->src_ ));
    return ap;
}


