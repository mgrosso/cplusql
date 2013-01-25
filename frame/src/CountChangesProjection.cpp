#ident "file_id $Id: CountChangesProjection.cpp,v 1.6 2004/09/21 21:30:48 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CountChangesProjection.cpp
// Author:    mgrosso 
// Created:   Tue Apr 15 03:12:56 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CountChangesProjection.cpp,v 1.6 2004/09/21 21:30:48 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "CountChangesProjection.h"


class CountChangesProjectionPriv 
{
    private:
    friend class CountChangesProjection ;
    CountChangesProjectionPriv( ExpressionPtr &src )
    :src_(src),prev_(ptr_strdup("")),first_(true)
    {
        //noop
    };
    ExpressionPtr   src_;
    char_ptr        prev_;
    bool            first_;
};

CountChangesProjection::CountChangesProjection( ExpressionPtr &src )
{
    d_=new CountChangesProjectionPriv ( src );
    result_=0;
}

CountChangesProjection::~CountChangesProjection()
{
    delete d_;
}

void
CountChangesProjection::childNotify()
{
    d_->src_->notify();
    char_ptr nval( d_->src_->getCharPtr() );
    if( d_->first_)
    {
        ++result_ ;
        d_->prev_=nval;
        d_->first_=false;
    }else if( strcmp( nval.get(), d_->prev_.get()))
    {
        ++result_;
        d_->prev_=nval;
    }
}

AbstractProjectionPtr
CountChangesProjection::copy()
{
    AbstractProjectionPtr ap( new CountChangesProjection( d_->src_ ));
    return ap;
}


