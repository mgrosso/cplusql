
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrLenExpression.h
// Author:    mgrosso 
// Created:   Fri Oct 29 01:48:04 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: StrLenExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRLENEXPRESSION_H
#define STRLENEXPRESSION_H 1

#include "BinaryIntegerExpression.h"
    
class StrLenExpression : public BinaryIntegerExpression
{
    public:
    ~StrLenExpression();
    StrLenExpression( ExpressionPtr operand );
    virtual long long           getLongLong() const ;
    virtual void                notify() ;

    private:
    //not implemented
    StrLenExpression();
    StrLenExpression( const StrLenExpression &rhs );
    StrLenExpression &operator=( const StrLenExpression &rhs );
};



#endif /* STRLENEXPRESSION_H */
