
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CompareListExpression.h
// Author:    mgrosso 
// Created:   Sat Jul 15 17:33:41 PDT 2006 on caliban
// Project:   
// Purpose:   Base class for Expressions that choose an Expression from 
//              an argument list.
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef COMPARELISTEXPRESSION_H
#define COMPARELISTEXPRESSION_H 1
    
#include "ProxyExpression.h"

class CompareListExpressionPriv ;
class CompareListExpression : public ProxyExpression
{
    public:
    CompareListExpression( ExpressionList &args );
    virtual ~CompareListExpression();
    virtual void                notify();

    protected:
    virtual void            childNotify();//for children to override.
    virtual ExpressionPtr & choose(ExpressionPtr &lhs, ExpressionPtr &rhs)=0;
    virtual ExpressionPtr & getProxy() const ;
    virtual ExpressionList & getList() ;

    private:
    CompareListExpressionPriv *d_;
    //not implemented
    CompareListExpression();
    CompareListExpression( const CompareListExpression &rhs );
    CompareListExpression &operator=( const CompareListExpression &rhs );
};



#endif /* COMPARELISTEXPRESSION_H */
