// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Thu Feb  6 07:00:47 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashExpression.h,v 1.6 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef HASHEXPRESSION_H
#define HASHEXPRESSION_H 1

#include "Expression.h"
#include "ptr.h"

class HashExpressionPriv  ;

class HashExpression : public Expression
{
    public:
    HashExpression( const ExpressionPtr &dsp, unsigned buckets );
    virtual ~HashExpression();

    virtual void                notify( void );

    virtual bool                isNull() const ;
    virtual const char *        getRaw() ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:
    HashExpression();
    HashExpressionPriv *d_;

    private:
    //not implemented
    HashExpression(const HashExpression &rhs);
    HashExpression &operator=(const HashExpression &rhs);
};

#endif /* COLUMNEXPRESSION_H */
