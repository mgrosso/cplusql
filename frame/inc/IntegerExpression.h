
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IntegerExpression.h
// Author:    mgrosso 
// Created:   Mon Apr 28 16:03:31 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: IntegerExpression.h,v 1.6 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef INTEGEREXPRESSION_H
#define INTEGEREXPRESSION_H 1

#include "Expression.h"

class IntegerExpressionPriv ;
class IntegerExpression : public Expression
{
    public:
    ~IntegerExpression();
    virtual void                notify( void );

    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const =0 ;
    virtual long double         getLongDouble() const ;

    protected:
    IntegerExpression();
    private:
    IntegerExpressionPriv *d_;
    //not implemented
    IntegerExpression( const IntegerExpression &rhs );
    IntegerExpression &operator=( const IntegerExpression &rhs );
};

#endif /* INTEGEREXPRESSION_H */
