#ident "file_id $Id: CountDistinctProjection.cpp,v 1.6 2004/09/21 21:30:48 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CountDistinctProjection.cpp
// Author:    mgrosso 
// Created:   Tue Apr 15 06:08:22 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CountDistinctProjection.cpp,v 1.6 2004/09/21 21:30:48 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CountDistinctProjection.h"
#include "ExpressionStringMap.h"

    

class CountDistinctProjectionPriv 
{
    private:
    friend class CountDistinctProjection ;
    CountDistinctProjectionPriv( ExpressionPtr &exp )
    :src_(exp)
    {
    };
    ExpressionPtr           src_;
    ExpressionStringMap     map_;
};

CountDistinctProjection::CountDistinctProjection( ExpressionPtr &exp )
{
    d_=new CountDistinctProjectionPriv ( exp );
    result_=0;
}

CountDistinctProjection::~CountDistinctProjection()
{
    delete d_;
}

void
CountDistinctProjection::childNotify()
{
    d_->src_->notify();
    d_->map_.set( d_->src_->getRaw(), "" );
    result_ = d_->map_.size();
}

AbstractProjectionPtr
CountDistinctProjection::copy()
{
    AbstractProjectionPtr ap( new CountDistinctProjection( d_->src_ ));
    return ap;
}


