#ident "file_id $Id: MinProjection.cpp,v 1.8 2004/09/21 23:50:37 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MinProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec 26 19:26:51 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MinProjection.cpp,v 1.8 2004/09/21 23:50:37 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MinProjection.h"
#include "BaseException.h"

class MinProjectionPriv 
{
    private:
    friend class MinProjection;

    MinProjectionPriv( ExpressionPtr &src )
    :src_(src),init_(false)
    {
        ;//noop
    };
    ExpressionPtr   src_;
    bool init_;
};

MinProjection::MinProjection( ExpressionPtr &src )
{
    d_=new MinProjectionPriv( src );
    if(!d_)
    {
        THROW_NOMEM;
    }
}

MinProjection::~MinProjection()
{
    delete d_;
}

void
MinProjection::childNotify(void)
{
    d_->src_->notify();
    long double sval = d_->src_->getLongDouble();
    if(!d_->init_ ) 
    {
        result_ = sval;
        d_->init_=true;
    }
    else if(sval < result_ )
    {
        result_ = sval;
    }
}

ptr< AbstractProjection >
MinProjection::copy()
{
    MinProjection *ap= new MinProjection( d_->src_ );
    if(!ap)
    {
        THROW_NOMEM;
    }
    return ptr< AbstractProjection > ( ap );
}

