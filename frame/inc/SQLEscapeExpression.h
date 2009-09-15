
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SQLEscapeExpression.h
// Author:    mgrosso 
// Created:   Sat Jan  8 16:28:42 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SQLEscapeExpression.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SQLESCAPEEXPRESSION_H
#define SQLESCAPEEXPRESSION_H 1

#include "RawExpression.h"

class SQLEscapeExpressionPriv ;
class SQLEscapeExpression : public RawExpression
{
    public:
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;
    virtual ~SQLEscapeExpression();
    SQLEscapeExpression( ExpressionPtr & operand );

    private:
    SQLEscapeExpressionPriv *d_;
    //not implemented
    SQLEscapeExpression();
    SQLEscapeExpression( const SQLEscapeExpression &rhs );
    SQLEscapeExpression &operator=( const SQLEscapeExpression &rhs );
};

#endif /* SQLESCAPEEXPRESSION_H */
