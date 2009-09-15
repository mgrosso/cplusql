#ident "file_id $Id: SumProjection.cpp,v 1.8 2004/09/21 23:56:14 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SumProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec 26 19:26:51 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SumProjection.cpp,v 1.8 2004/09/21 23:56:14 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SumProjection.h"
#include "BaseException.h"

class SumProjectionPriv 
{
    private:
    friend class SumProjection;

    SumProjectionPriv( ExpressionPtr &src )
    :src_(src)
    {
        ;//noop
    };
    ExpressionPtr   src_;
};

SumProjection::SumProjection( ExpressionPtr &src )
{
    d_=new SumProjectionPriv( src );
}

SumProjection::~SumProjection()
{
    delete d_;
}

void
SumProjection::childNotify()
{
    d_->src_->notify();
    result_ += d_->src_->getLongDouble();
}

ptr< AbstractProjection >
SumProjection::copy()
{
    SumProjection *ap= new SumProjection( d_->src_ );
    return ptr< AbstractProjection > ( ap );
}

