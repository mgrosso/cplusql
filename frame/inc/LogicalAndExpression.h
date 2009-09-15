// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogicalAndExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:32:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LogicalAndExpression.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef LOGICALANDEXPRESSION_H
#define LOGICALANDEXPRESSION_H 1

#include "LogicalExpression.h"

class LogicalAndExpression : public LogicalExpression 
{
    public:
    LogicalAndExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual                     ~LogicalAndExpression();
    virtual void                notify();
    virtual bool                getBool() const ;
    protected:
    private:
    bool value_ ;
    //not implemented
    LogicalAndExpression();
    LogicalAndExpression(const LogicalAndExpression &rhs);
    LogicalAndExpression &operator=(const LogicalAndExpression &rhs);
};

#endif /* LOGICALANDEXPRESSION_H */
