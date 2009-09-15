
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MD5Expression.h
// Author:    mgrosso 
// Created:   Thu Jun 29 04:10:01 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef MD5EXPRESSION_H
#define MD5EXPRESSION_H 1
#include "RawExpression.h"
    
class MD5ExpressionPriv ;
class MD5Expression : public RawExpression
{
    public:
    MD5Expression( ExpressionPtr &operand );
    virtual                     ~MD5Expression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    MD5ExpressionPriv *d_;
    //not implemented
    MD5Expression();
    MD5Expression( const MD5Expression &rhs );
    MD5Expression &operator=( const MD5Expression &rhs );
};

#endif /* MD5EXPRESSION_H */
