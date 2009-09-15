
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FactorialExpression.h
// Author:    mgrosso 
// Created:   Thu Apr 14 13:01:59 EDT 2005 on caliban
// Project:   
// Purpose:   
// 
// $Id: FactorialExpression.h,v 1.1 2005/10/19 08:41:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef FACTORIALEXPRESSION_H
#define FACTORIALEXPRESSION_H 1

#include "IntegerExpression.h"

class FactorialExpressionPriv ;
class FactorialExpression : public IntegerExpression 
{
    public:
    FactorialExpression( ExpressionPtr &operand );
    virtual ~FactorialExpression();
    virtual long long           getLongLong() const ;

    private:
    FactorialExpressionPriv *d_;
    //not implemented
    FactorialExpression();
    FactorialExpression( const FactorialExpression &rhs );
    FactorialExpression &operator=( const FactorialExpression &rhs );
};



#endif /* FACTORIALEXPRESSION_H */
