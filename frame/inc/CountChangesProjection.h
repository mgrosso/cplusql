// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CountChangesProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: CountChangesProjection.h,v 1.4 2003/10/28 22:22:40 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef COUNTCHANGESPROJECTION_H
#define COUNTCHANGESPROJECTION_H 1

#include "NumericProjection.h"

class CountChangesProjectionPriv ;

class CountChangesProjection : public NumericProjection
{
    public:
    CountChangesProjection( ExpressionPtr &src );
    virtual ~CountChangesProjection();
    virtual void                        childNotify();
    virtual ptr< AbstractProjection >    copy();

    //from Expression

    private:
    CountChangesProjectionPriv  *d_;
    
    CountChangesProjection();
    CountChangesProjection(const CountChangesProjection &rhs);
    CountChangesProjection &operator=(const CountChangesProjection &rhs);
};



#endif /* COUNTCHANGESPROJECTION_H */
