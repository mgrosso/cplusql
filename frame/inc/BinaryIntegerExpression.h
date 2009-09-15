
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BinaryIntegerExpression.h
// Author:    mgrosso 
// Created:   Fri Oct 29 00:36:09 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: BinaryIntegerExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef BINARYINTEGEREXPRESSION_H
#define BINARYINTEGEREXPRESSION_H 1
    
#include "IntegerExpression.h"

class BinaryExpressionPriv ;
class BinaryIntegerExpression : public IntegerExpression
{
    public:
    ~BinaryIntegerExpression();
    virtual void                notify( void );

    protected:
    BinaryIntegerExpression( ExpressionPtr &lhs, ExpressionPtr &rhs );
    BinaryExpressionPriv *d_;

    private:
    //not implemented
    BinaryIntegerExpression();
    BinaryIntegerExpression( const BinaryIntegerExpression &rhs );
    BinaryIntegerExpression &operator=( const BinaryIntegerExpression &rhs);
};



#endif /* BINARYINTEGEREXPRESSION_H */
