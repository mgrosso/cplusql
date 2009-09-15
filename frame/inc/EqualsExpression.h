// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      EqualsExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 23:19:04 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: EqualsExpression.h,v 1.1 2003/01/22 05:45:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef EQUALSEXPRESSION_H
#define EQUALSEXPRESSION_H 1

#include "LogicalExpression.h"

class EqualsExpression : public LogicalExpression 
{
    public:
    EqualsExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual ~EqualsExpression();
    virtual bool                getBool() const ;
    protected:
    private:
    //not implemented
    EqualsExpression();
    EqualsExpression(const EqualsExpression &rhs);
    EqualsExpression &operator=(const EqualsExpression &rhs);
};

#endif /* EQUALSEXPRESSION_H */
