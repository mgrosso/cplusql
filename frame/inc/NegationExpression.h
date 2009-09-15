// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NegationExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:55:53 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: NegationExpression.h,v 1.5 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef NEGATIONEXPRESSION_H
#define NEGATIONEXPRESSION_H 1

#include "BooleanExpression.h"
#include "UnaryExpression.h"

class NegationExpression : public BooleanExpression
{
    public:
    NegationExpression( ExpressionPtr &operand );
    virtual ~NegationExpression();
    virtual void                notify() ;
    virtual bool                isNull() const ;
    virtual bool                getBool() const ;

    protected:
    private:
    ExpressionPtr operand_ ;
    //not implemented
    NegationExpression();
    NegationExpression(const NegationExpression &rhs);
    NegationExpression &operator=(const NegationExpression &rhs);
};

#endif /* NEGATIONEXPRESSION_H */
