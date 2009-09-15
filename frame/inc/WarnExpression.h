
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      WarnExpression.h
// Author:    mgrosso 
// Created:   Sun Nov 21 18:23:32 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: WarnExpression.h,v 1.1 2005/01/05 01:04:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef WARNEXPRESSION_H
#define WARNEXPRESSION_H 1
    
#include "RawExpression.h"

class WarnExpressionPriv ;
class WarnExpression : public RawExpression
{
    public:
    WarnExpression(ExpressionPtr &operand );
    virtual ~WarnExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    ExpressionPtr operand_;

    //not implemented
    WarnExpression();
    WarnExpression( const WarnExpression &rhs );
    WarnExpression &operator=( const WarnExpression &rhs );
};



#endif /* WARNEXPRESSION_H */
