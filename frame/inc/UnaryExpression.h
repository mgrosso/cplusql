// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      UnaryExpression.h
// Author:    mgrosso 
// Created:   Tue Jun  3 23:00:57 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: UnaryExpression.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef UNARYEXPRESSION_H
#define UNARYEXPRESSION_H 1
    

#include "Expression.h"

class UnaryExpression : public Expression
{
    public:
    UnaryExpression( ExpressionPtr &e );
    virtual ~UnaryExpression();
    virtual void    notify(); 
    virtual bool    isNull() const ;

    protected:
    ExpressionPtr operand_ ;

    private:
    //not implemented
    UnaryExpression();
    UnaryExpression( const UnaryExpression &rhs );
    UnaryExpression &operator=( const UnaryExpression &rhs );
};



#endif /* UNARYEXPRESSION_H */

