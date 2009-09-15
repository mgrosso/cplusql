
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrGreatestExpression.h
// Author:    mgrosso 
// Created:   Sat Jul 15 18:38:37 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRGREATESTEXPRESSION_H
#define STRGREATESTEXPRESSION_H 1

#include "CompareListExpression.h"
    
class StrGreatestExpression : public CompareListExpression
{
    public:
    StrGreatestExpression( ExpressionList &args );
    virtual ~StrGreatestExpression();
    virtual ExpressionPtr & choose(ExpressionPtr &lhs, ExpressionPtr &rhs);

    private:
    //not implemented
    StrGreatestExpression();
    StrGreatestExpression( const StrGreatestExpression &rhs );
    StrGreatestExpression &operator=( const StrGreatestExpression &rhs );
};



#endif /* STRGREATESTEXPRESSION_H */
