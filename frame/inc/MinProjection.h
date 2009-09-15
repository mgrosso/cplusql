// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MinProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MinProjection.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MINPROJECTION_H
#define MINPROJECTION_H 1

#include "DoubleProjection.h"

class MinProjectionPriv ;

class MinProjection : public DoubleProjection
{
    public:
    MinProjection( ExpressionPtr &src );
    virtual ~MinProjection();
    virtual void                        childNotify();
    virtual ptr< AbstractProjection >    copy();

    //from Expression

    private:
    MinProjectionPriv  *d_;
    
    MinProjection();
    MinProjection(const MinProjection &rhs);
    MinProjection &operator=(const MinProjection &rhs);
};



#endif /* MINPROJECTION_H */
