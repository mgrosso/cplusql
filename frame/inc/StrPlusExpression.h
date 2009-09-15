
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrPlusExpression.h
// Author:    mgrosso 
// Created:   Fri Oct 29 02:13:26 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: StrPlusExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRPLUSEXPRESSION_H
#define STRPLUSEXPRESSION_H 1
#include "RawExpression.h"

class StrPlusExpressionPriv ;
class StrPlusExpression: public RawExpression
{
    public:
    StrPlusExpression( ExpressionPtr &lhs,  ExpressionPtr &rhs);
    ~StrPlusExpression();
    virtual void                notify() ;
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;

    private:
    StrPlusExpressionPriv *d_;
    //not implemented
    StrPlusExpression();
    StrPlusExpression( const StrPlusExpression &rhs );
    StrPlusExpression &operator=( const StrPlusExpression &rhs );
};

#endif /* STRPLUSEXPRESSION_H */
