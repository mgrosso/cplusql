// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Expression.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Nov  8 00:08:18 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: Expression.h,v 1.7 2005/02/22 07:14:29 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef EXPRESSION_H
#define EXPRESSION_H 1

#include "ptr.h"
#include "ptr_strdup.h"
#include <vector>
#include "Notifier.h"

class BaseExpression : public Listener
{
    public:
    virtual void                notify(void);
    virtual void                notifyLast(void);

    virtual bool                isNull()const=0 ;
    virtual const char *        getRaw()=0 ;
    virtual char_ptr            getCharPtr()const=0 ;
    virtual bool                getBool()const=0 ;
    virtual long int            getLong()const=0 ;
    virtual double              getDouble()const=0 ;
    virtual long long           getLongLong()const=0 ;
    virtual long double         getLongDouble()const=0 ;

    virtual ~BaseExpression();
    protected:
    BaseExpression();

    private:
    BaseExpression(const BaseExpression &rhs);
    BaseExpression &operator=(const BaseExpression &rhs);
};

typedef BaseExpression Expression ;

typedef ptr< BaseExpression > ExpressionPtr ;
typedef vector< ptr< BaseExpression > > expression_list_t ;
typedef ptr< vector< ptr< BaseExpression > > > ExpressionList ;
typedef expression_list_t::iterator ExpressionListItr ;
typedef expression_list_t::const_iterator ExpressionListConstItr ;

#endif /* EXPRESSION_H */
