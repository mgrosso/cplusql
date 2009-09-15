
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrLeastExpression.h
// Author:    mgrosso 
// Created:   Sat Jul 15 18:59:14 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRLEASTEXPRESSION_H
#define STRLEASTEXPRESSION_H 1


#include "CompareListExpression.h"
    
class StrLeastExpression : public CompareListExpression
{
    public:
    StrLeastExpression( ExpressionList &args );
    virtual ~StrLeastExpression();
    virtual ExpressionPtr & choose(ExpressionPtr &lhs, ExpressionPtr &rhs);

    private:
    //not implemented
    StrLeastExpression();
    StrLeastExpression( const StrLeastExpression &rhs );
    StrLeastExpression &operator=( const StrLeastExpression &rhs );
};



#endif /* STRLEASTEXPRESSION_H */
