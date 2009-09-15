// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrGreaterThanExpression.h
// Author:    mgrosso 
// Created:   Fri Jul 14 17:56:33 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRGREATERTHANEXPRESSION_H
#define STRGREATERTHANEXPRESSION_H 1

#include "LogicalExpression.h"
    
class StrGreaterThanExpressionPriv ;
class StrGreaterThanExpression : public LogicalExpression 
{
    public:
    StrGreaterThanExpression(ExpressionPtr lhs, ExpressionPtr rhs );
    virtual ~StrGreaterThanExpression();
    virtual bool                getBool() const ;

    private:
    //not implemented
    StrGreaterThanExpression();
    StrGreaterThanExpression( const StrGreaterThanExpression &rhs );
    StrGreaterThanExpression &operator=( const StrGreaterThanExpression &rhs );
};



#endif /* STRGREATERTHANEXPRESSION_H */
