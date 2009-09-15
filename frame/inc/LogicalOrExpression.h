// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogicalOrExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:32:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LogicalOrExpression.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef LOGICALOREXPRESSION_H
#define LOGICALOREXPRESSION_H 1

#include "LogicalExpression.h"

class LogicalOrExpression : public LogicalExpression 
{
    public:
    LogicalOrExpression( ExpressionPtr lhs, ExpressionPtr rhs );
    virtual                     ~LogicalOrExpression();
    virtual bool                getBool() const ;
    virtual void                notify();
    protected:
    private:
    bool value_;
    //not implemented
    LogicalOrExpression();
    LogicalOrExpression(const LogicalOrExpression &rhs);
    LogicalOrExpression &operator=(const LogicalOrExpression &rhs);
};

#endif /* LOGICALOREXPRESSION_H */
