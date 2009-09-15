#ident "file_id $Id: FactorialExpression.cpp,v 1.1 2005/10/19 08:41:19 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FactorialExpression.cpp
// Author:    mgrosso 
// Created:   Thu Apr 14 13:01:59 EDT 2005 on caliban
// Project:   
// Purpose:   
// 
// $Id: FactorialExpression.cpp,v 1.1 2005/10/19 08:41:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "FactorialExpression.h"

    

class FactorialExpressionPriv 
{
    private:
    friend class FactorialExpression ;
    ExpressionPtr                       operand_ ;
    FactorialExpressionPriv( 
        ExpressionPtr &operand )
    :operand_(operand)
    {
        ;//noop
    };
};

FactorialExpression::FactorialExpression( ExpressionPtr &operand )
{
    d_=new FactorialExpressionPriv (operand );
}

FactorialExpression::~FactorialExpression()
{
    delete d_;
}

long long 
FactorialExpression::getLongLong() const
{
    long long x = d_->operand_->getLongLong();
    long long r=x;
    while(--x)
    {
        r=r*x;
    }
    return r;
}




