// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AdditionExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:31:30 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AdditionExpression.h,v 1.3 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ADDITIONEXPRESSION_H
#define ADDITIONEXPRESSION_H 1

#include "Expression.h"
#include "ArithmeticExpression.h"


class AdditionExpression : public ArithmeticExpression 
{
    public:
    AdditionExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    virtual ~AdditionExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:

    private:
    //not implemented
    AdditionExpression();
    AdditionExpression(const AdditionExpression &rhs);
    AdditionExpression &operator=(const AdditionExpression &rhs);
};

#endif /* ADDITIONEXPRESSION_H */
