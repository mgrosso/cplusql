// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      GreaterThanExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:32:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: GreaterThanExpression.h,v 1.1 2004/05/14 14:34:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef GREATERTHANEXPRESSION_H
#define GREATERTHANEXPRESSION_H 1

#include "LogicalExpression.h"

class GreaterThanExpression : public LogicalExpression 
{
    public:
    GreaterThanExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual ~GreaterThanExpression();
    virtual bool                getBool() const ;
    protected:
    private:
    //not implemented
    GreaterThanExpression();
    GreaterThanExpression(const GreaterThanExpression &rhs);
    GreaterThanExpression &operator=(const GreaterThanExpression &rhs);
};

#endif /* GREATERTHANEXPRESSION_H */
