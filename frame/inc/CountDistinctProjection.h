// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CountDistinctProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: CountDistinctProjection.h,v 1.4 2003/10/28 22:22:40 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef COUNTDISTINCTPROJECTION_H
#define COUNTDISTINCTPROJECTION_H 1

#include "NumericProjection.h"

class CountDistinctProjectionPriv ;

class CountDistinctProjection : public NumericProjection
{
    public:
    CountDistinctProjection( ExpressionPtr &src );
    virtual ~CountDistinctProjection();
    virtual void                        childNotify();
    virtual ptr< AbstractProjection >    copy();

    //from Expression

    private:
    CountDistinctProjectionPriv  *d_;
    
    CountDistinctProjection();
    CountDistinctProjection(const CountDistinctProjection &rhs);
    CountDistinctProjection &operator=(const CountDistinctProjection &rhs);
};



#endif /* COUNTCHANGESPROJECTION_H */
