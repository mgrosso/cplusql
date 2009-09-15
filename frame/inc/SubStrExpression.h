
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SubStrExpression.h
// Author:    mgrosso 
// Created:   Wed Jan 12 17:00:01 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SubStrExpression.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SUBSTREXPRESSION_H
#define SUBSTREXPRESSION_H 1

#include "RawExpression.h"
    
class SubStrExpressionPriv ;
class SubStrExpression : virtual public RawExpression
{
    public:
    SubStrExpression( 
        ExpressionPtr &operand,
        ExpressionPtr &start,
        ExpressionPtr &length
    );
    virtual ~SubStrExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    SubStrExpressionPriv *d_;
    //not implemented
    SubStrExpression();
    SubStrExpression( const SubStrExpression &rhs );
    SubStrExpression &operator=( const SubStrExpression &rhs );
};

#endif /* SUBSTREXPRESSION_H */
