// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LessThanExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:32:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LessThanExpression.h,v 1.1 2003/01/22 05:45:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef LESSTHANEXPRESSION_H
#define LESSTHANEXPRESSION_H 1

#include "LogicalExpression.h"

class LessThanExpression : public LogicalExpression 
{
    public:
    LessThanExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual ~LessThanExpression();
    virtual bool                getBool() const ;
    protected:
    private:
    //not implemented
    LessThanExpression();
    LessThanExpression(const LessThanExpression &rhs);
    LessThanExpression &operator=(const LessThanExpression &rhs);
};

#endif /* LESSTHANEXPRESSION_H */
