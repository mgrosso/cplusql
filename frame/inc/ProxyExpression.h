// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyExpression.h
// Author:    mgrosso 
// Created:   Mon Mar 31 05:50:52 EST 2003 on caliban.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ProxyExpression.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PROXYEXPRESSION_H
#define PROXYEXPRESSION_H 1

#include "Expression.h"

class ProxyExpression : public Expression
{
    public:
    ~ProxyExpression();
    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;
 
    protected:
    ProxyExpression();
    virtual ExpressionPtr &              getProxy() const =0;

    private:
    //not implemented
    ProxyExpression( const ProxyExpression &rhs );
    ProxyExpression &operator=( const ProxyExpression &rhs );
};

#endif /* PROXYEXPRESSION_H */

