
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CommandExpression.h
// Author:    mgrosso 
// Created:   Sun Jan  2 03:21:40 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: CommandExpression.h,v 1.1 2005/01/03 01:04:14 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef COMMANDEXPRESSION_H
#define COMMANDEXPRESSION_H 1

#include "RawExpression.h"

class CommandExpressionPriv ;
class CommandExpression : public RawExpression
{
    public:
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    CommandExpression( ExpressionPtr &operand, bool panic_on_failure );
    virtual ~CommandExpression();

    private:
    CommandExpressionPriv *d_;
    //not implemented
    CommandExpression();
    CommandExpression( const CommandExpression &rhs );
    CommandExpression &operator=( const CommandExpression &rhs );
};

#endif /* COMMANDEXPRESSION_H */
