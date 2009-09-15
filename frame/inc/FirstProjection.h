
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FirstProjection.h
// Author:    mgrosso 
// Created:   Tue Apr 15 07:00:01 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: FirstProjection.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef FIRSTPROJECTION_H
#define FIRSTPROJECTION_H 1
    
#include "StringProjection.h"

class FirstProjectionPriv ;
class FirstProjection : public StringProjection
{
    public:
    FirstProjection( ExpressionPtr & exp );
    ~FirstProjection();
    virtual     void                            childNotify();
    virtual     AbstractProjectionPtr        copy();

    protected:

    private:
    FirstProjectionPriv *d_;
    //not implemented
    FirstProjection();
    FirstProjection( const FirstProjection &rhs );
    FirstProjection &operator=( const FirstProjection &rhs );
};



#endif /* FIRSTPROJECTION_H */
