
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ChompExpression.h
// Author:    mgrosso 
// Created:   Sun Jan  2 16:13:27 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: ChompExpression.h,v 1.1 2005/01/03 01:04:14 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef CHOMPEXPRESSION_H
#define CHOMPEXPRESSION_H 1
#include "RawExpression.h"
    
class ChompExpressionPriv ;
class ChompExpression : public RawExpression
{
    public:
    ChompExpression( ExpressionPtr &operand );
    virtual                     ~ChompExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    ChompExpressionPriv *d_;
    //not implemented
    ChompExpression();
    ChompExpression( const ChompExpression &rhs );
    ChompExpression &operator=( const ChompExpression &rhs );
};



#endif /* CHOMPEXPRESSION_H */
