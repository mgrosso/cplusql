#ident "file_id $Id: MaxProjection.cpp,v 1.8 2004/09/21 23:50:36 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MaxProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec 26 19:26:51 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MaxProjection.cpp,v 1.8 2004/09/21 23:50:36 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MaxProjection.h"
#include "BaseException.h"

class MaxProjectionPriv 
{
    private:
    friend class MaxProjection;

    MaxProjectionPriv( ExpressionPtr &src )
    :src_(src),init_(false)
    {
        ;//noop
    };
    ExpressionPtr   src_;
    bool init_;
};

MaxProjection::MaxProjection( ExpressionPtr &src )
{
    d_=new MaxProjectionPriv( src );
    if(!d_)
    {
        THROW_NOMEM;
    }
}

MaxProjection::~MaxProjection()
{
    delete d_;
}

void
MaxProjection::childNotify()
{
    d_->src_->notify();
    long double sval = d_->src_->getLongDouble();
    if(!d_->init_ ) 
    {
        result_ = sval;
        d_->init_=true;
    }
    else if(sval > result_ )
    {
        result_ = sval;
    }
}

ptr< AbstractProjection >
MaxProjection::copy()
{
    MaxProjection *ap= new MaxProjection( d_->src_ );
    if(!ap)
    {
        THROW_NOMEM;
    }
    return ptr< AbstractProjection > ( ap );
}

