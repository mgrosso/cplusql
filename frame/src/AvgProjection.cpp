#ident "file_id $Id: AvgProjection.cpp,v 1.1 2004/09/20 23:26:07 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AvgProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec 26 19:26:51 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AvgProjection.cpp,v 1.1 2004/09/20 23:26:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AvgProjection.h"

class AvgProjectionPriv 
{
    private:
    friend class AvgProjection;

    AvgProjectionPriv( ExpressionPtr &src )
    :src_(src),sum_(0),cnt_(0)
    {
        ;//noop
    };
    ExpressionPtr   src_;
    long double     sum_;
    long double     cnt_;
};

AvgProjection::AvgProjection( ExpressionPtr &src )
{
    d_=new AvgProjectionPriv( src );
}

AvgProjection::~AvgProjection()
{
    delete d_;
}

void
AvgProjection::childNotify()
{
    d_->src_->notify();
    d_->sum_ += d_->src_->getLongDouble();
    ++ d_->cnt_ ;
    result_ = d_->sum_ / d_->cnt_ ;
}

ptr< AbstractProjection >
AvgProjection::copy()
{
    AvgProjection *ap= new AvgProjection( d_->src_ );
    return ptr< AbstractProjection > ( ap );
}

