// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrLessThanExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:32:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: StrLessThanExpression.h,v 1.1 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef STRLESSTHANEXPRESSION_H
#define STRLESSTHANEXPRESSION_H 1

#include "LogicalExpression.h"

class StrLessThanExpression : public LogicalExpression 
{
    public:
    StrLessThanExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual ~StrLessThanExpression();
    virtual bool                getBool() const ;
    protected:
    private:
    //not implemented
    StrLessThanExpression();
    StrLessThanExpression(const StrLessThanExpression &rhs);
    StrLessThanExpression &operator=(const StrLessThanExpression &rhs);
};

#endif /* STRLESSTHANEXPRESSION_H */
