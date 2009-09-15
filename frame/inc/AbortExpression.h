// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbortExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:08:07 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbortExpression.h,v 1.4 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABORTEXPRESSION_H
#define ABORTEXPRESSION_H 1

#include "UnaryExpression.h"

class AbortExpression : public UnaryExpression 
{
    public:
    AbortExpression(ExpressionPtr &e);
    virtual ~AbortExpression();
    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:

    private:
    void doThrow() const;

    //not implemented
    AbortExpression();
    AbortExpression(const AbortExpression &rhs);
    AbortExpression &operator=(const AbortExpression &rhs);
};

#endif /* ABORTEXPRESSION_H */
