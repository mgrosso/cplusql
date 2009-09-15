// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FloorExpression.h
// Author:    djain Deepak Jain
// Created:   Thu Apr 14 09:00:47 EST 2003 on LS-11452
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: FloorExpression.h,v 1.1 2004/05/05 23:03:14 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef FLOOREXPRESSION_H
#define FLOOREXPRESSION_H 1

#include "HashExpression.h"
#include "ptr.h"

class HashExpressionPriv  ;

class FloorExpression : public HashExpression
{
    public:
    FloorExpression( const ExpressionPtr &dsp );
    virtual ~FloorExpression();

    protected:

    private:
    //not implemented
    FloorExpression();
    FloorExpression(const FloorExpression &rhs);
    FloorExpression &operator=(const FloorExpression &rhs);
};

#endif /* FLOOREXPRESSION_H */
