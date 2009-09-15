// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CountProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: CountProjection.h,v 1.4 2003/10/28 22:22:40 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef COUNTPROJECTION_H
#define COUNTPROJECTION_H 1

#include "NumericProjection.h"

class CountProjection : public NumericProjection
{
    public:
    CountProjection();
    virtual ~CountProjection();
    virtual void                        childNotify();
    virtual ptr< AbstractProjection >    copy();

    //from Expression

    private:
    CountProjection(const CountProjection &rhs);
    CountProjection &operator=(const CountProjection &rhs);
};



#endif /* COUNTPROJECTION_H */
