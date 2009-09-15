// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CharExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:08:07 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: CharExpression.h,v 1.3 2003/07/10 19:43:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CHAREXPRESSION_H
#define CHAREXPRESSION_H 1

#include "Expression.h"

class CharExpression : public Expression 
{
    public:
    CharExpression(const char *value );
    virtual ~CharExpression();
            void                set( const char *value );
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
    const char *d_;
    //not implemented
    CharExpression();
    CharExpression(const CharExpression &rhs);
    CharExpression &operator=(const CharExpression &rhs);
};

#endif /* CHAREXPRESSION_H */
