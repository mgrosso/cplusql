// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ModuloExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:32:05 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ModuloExpression.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MODULOEXPRESSION_H
#define MODULOEXPRESSION_H 1

#include "Expression.h"
#include "ArithmeticExpression.h"

class ModuloExpression : public ArithmeticExpression 
{
    public:
    ModuloExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    virtual ~ModuloExpression();
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:

    private:
    //not implemented
    ModuloExpression();
    ModuloExpression(const ModuloExpression &rhs);
    ModuloExpression &operator=(const ModuloExpression &rhs);
};

#endif /* MODULOEXPRESSION_H */
