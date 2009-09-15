// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MaxProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MaxProjection.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MAXPROJECTION_H
#define MAXPROJECTION_H 1

#include "DoubleProjection.h"

class MaxProjectionPriv ;

class MaxProjection : public DoubleProjection
{
    public:
    MaxProjection( ExpressionPtr &src );
    virtual ~MaxProjection();
    virtual void                        childNotify();
    virtual ptr< AbstractProjection >    copy();

    //from Expression

    private:
    MaxProjectionPriv  *d_;
    
    MaxProjection();
    MaxProjection(const MaxProjection &rhs);
    MaxProjection &operator=(const MaxProjection &rhs);
};



#endif /* MAXPROJECTION_H */
