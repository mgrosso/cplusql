
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LogInfoExpression.h
// Author:    mgrosso 
// Created:   Tue Jul 18 04:09:51 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LOGINFOEXPRESSION_H
#define LOGINFOEXPRESSION_H 1
#include "RawExpression.h"
    
class LogInfoExpressionPriv ;
class LogInfoExpression : public RawExpression
{
    public:
                                LogInfoExpression(ExpressionPtr &operand );
    virtual                     ~LogInfoExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    LogInfoExpressionPriv *d_;
    //not implemented
    LogInfoExpression();
    LogInfoExpression( const LogInfoExpression &rhs );
    LogInfoExpression &operator=( const LogInfoExpression &rhs );
};



#endif /* LOGINFOEXPRESSION_H */
