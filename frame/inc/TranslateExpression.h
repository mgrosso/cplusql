
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      TranslateExpression.h
// Author:    mgrosso 
// Created:   Wed May 26 00:54:29 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: TranslateExpression.h,v 1.1 2004/06/02 20:39:31 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LOWERCASEEXPRESSION_H
#define LOWERCASEEXPRESSION_H 1

#include "RawExpression.h"

class TranslateExpressionPriv ;
class TranslateExpression : public RawExpression
{
    public:
    typedef int (*translator_t)(int) ;
    TranslateExpression( ExpressionPtr &operand, translator_t f );
    virtual ~TranslateExpression();
    virtual void                notify() ;
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;

    private:
    TranslateExpressionPriv *d_;
    //not implemented
    TranslateExpression();
    TranslateExpression( const TranslateExpression &rhs );
    TranslateExpression &operator=( const TranslateExpression &rhs );
};



#endif /* LOWERCASEEXPRESSION_H */
