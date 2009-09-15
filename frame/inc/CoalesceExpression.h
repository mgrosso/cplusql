
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CoalesceExpression.h
// Author:    mgrosso 
// Created:   Mon Jan 10 07:00:00 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: CoalesceExpression.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef COALESCEEXPRESSION_H
#define COALESCEEXPRESSION_H 1

#include "RawExpression.h"

class CoalesceExpressionPriv ;
class CoalesceExpression : public RawExpression
{
    public:
    CoalesceExpression( ExpressionList &args );
    virtual ~CoalesceExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    CoalesceExpressionPriv *d_;
    //not implemented
    CoalesceExpression();
    CoalesceExpression( const CoalesceExpression &rhs );
    CoalesceExpression &operator=( const CoalesceExpression &rhs );
};



#endif /* COALESCEEXPRESSION_H */
