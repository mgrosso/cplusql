
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RawExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 20:27:28 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: RawExpression.h,v 1.1 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef RAWEXPRESSION_H
#define RAWEXPRESSION_H 1

#include "Expression.h"
    
class RawExpression : public Expression
{
    public:
    virtual ~RawExpression();

    //subclass these two methods, and let 
    virtual const char *        getRaw() =0 ;
    virtual const char *        getRaw() const =0 ;
    virtual void                notify() =0 ;

    virtual bool                isNull() const ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    protected:
    RawExpression();

    private:
    //not implemented
    RawExpression( const RawExpression &rhs );
    RawExpression &operator=( const RawExpression &rhs );
};

#endif /* RAWEXPRESSION_H */
