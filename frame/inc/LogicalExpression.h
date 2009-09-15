// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogicalExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:55:53 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LogicalExpression.h,v 1.6 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef LOGICALEXPRESSION_H
#define LOGICALEXPRESSION_H 1

#include "BooleanExpression.h"

class BinaryExpressionPriv  ;

class LogicalExpression : public BooleanExpression 
{
    public:
    LogicalExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual void                notify();
    virtual                     ~LogicalExpression();
    virtual bool                isNull() const ;
    virtual bool                getBool()const =0 ;

    protected:
    BinaryExpressionPriv *d_;
    private:
    //not implemented
    LogicalExpression();
    LogicalExpression(const LogicalExpression &rhs);
    LogicalExpression &operator=(const LogicalExpression &rhs);
};

#endif /* LOGICALEXPRESSION_H */
