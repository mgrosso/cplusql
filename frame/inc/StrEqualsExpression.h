// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrEqualsExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 23:19:04 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: StrEqualsExpression.h,v 1.1 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef STREQUALSEXPRESSION_H
#define STREQUALSEXPRESSION_H 1

#include "LogicalExpression.h"

class StrEqualsExpression : public LogicalExpression 
{
    public:
    StrEqualsExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual ~StrEqualsExpression();
    virtual bool                getBool() const ;
    protected:
    private:
    //not implemented
    StrEqualsExpression();
    StrEqualsExpression(const StrEqualsExpression &rhs);
    StrEqualsExpression &operator=(const StrEqualsExpression &rhs);
};

#endif /* STREQUALSEXPRESSION_H */
