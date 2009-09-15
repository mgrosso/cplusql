// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BooleanExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:55:53 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: BooleanExpression.h,v 1.3 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef BOOLEANEXPRESSION_H
#define BOOLEANEXPRESSION_H 1

#include "Expression.h"

class BooleanExpression : public Expression 
{
    public:
    virtual ~BooleanExpression();
    virtual bool                isNull() const =0 ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool()const =0 ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:
    BooleanExpression();
    private:
    static const char *const    false_raw ;
    static const char *const    true_raw ;
    static char_ptr             false_rawp ;
    static char_ptr             true_rawp ;
    //not implemented
    BooleanExpression(const BooleanExpression &rhs);
    BooleanExpression &operator=(const BooleanExpression &rhs);
};

#endif /* BOOLEANEXPRESSION_H */
