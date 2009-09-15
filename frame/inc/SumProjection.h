// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SumProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SumProjection.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SUMPROJECTION_H
#define SUMPROJECTION_H 1

#include "DoubleProjection.h"

class SumProjectionPriv ;

class SumProjection : public DoubleProjection
{
    public:
    SumProjection( ExpressionPtr &src );
    virtual ~SumProjection();
    virtual void                        childNotify();
    virtual ptr< AbstractProjection >    copy();

    //from Expression

    private:
    SumProjectionPriv  *d_;
    
    SumProjection();
    SumProjection(const SumProjection &rhs);
    SumProjection &operator=(const SumProjection &rhs);
};



#endif /* SUMPROJECTION_H */
