// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IfThenExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:31:30 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: IfThenExpression.h,v 1.5 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef IFTHENEXPRESSION_H
#define IFTHENEXPRESSION_H 1

#include "ProxyExpression.h"

class IfThenExpressionPriv ;

class IfThenExpression : public ProxyExpression 
{
    public:
    IfThenExpression( 
        ExpressionPtr &condition, 
        ExpressionPtr &texp, 
        ExpressionPtr &fexp 
    );
    virtual ~IfThenExpression();
    virtual void    notify( void );

    protected:
    virtual ExpressionPtr &     getProxy() const ;
    IfThenExpressionPriv *d_;
    private:
    //not implemented
    IfThenExpression();
    IfThenExpression(const IfThenExpression &rhs);
    IfThenExpression &operator=(const IfThenExpression &rhs);
};

#endif /* IFTHENEXPRESSION_H */
