
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LastProjection.h
// Author:    mgrosso 
// Created:   Tue Apr 15 07:00:01 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: LastProjection.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LASTPROJECTION_H
#define LASTPROJECTION_H 1
    
#include "StringProjection.h"

class LastProjectionPriv ;
class LastProjection : public StringProjection
{
    public:
    LastProjection( ExpressionPtr & exp );
    ~LastProjection();
    virtual     void                            childNotify();
    virtual     AbstractProjectionPtr        copy();

    protected:

    private:
    LastProjectionPriv *d_;
    //not implemented
    LastProjection();
    LastProjection( const LastProjection &rhs );
    LastProjection &operator=( const LastProjection &rhs );
};



#endif /* LASTPROJECTION_H */
