
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexMatchExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 19:05:55 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: RegexMatchExpression.h,v 1.2 2004/05/13 23:06:59 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef REGEXMATCHEXPRESSION_H
#define REGEXMATCHEXPRESSION_H 1

#include "BooleanExpression.h"
    
class RegexMatchExpressionPriv ;
class RegexMatchExpression : public BooleanExpression
{
    public:
    RegexMatchExpression( 
        ExpressionPtr &operand, 
        const char *pattern, 
        bool case_insensitive  );

    ~RegexMatchExpression();

    virtual bool isNull() const ;
    virtual bool getBool() const ;
    virtual void notify(); 
    virtual bool match( const char *operand ) const;
    private:
    RegexMatchExpressionPriv *d_;
    //not implemented
    RegexMatchExpression();
    RegexMatchExpression( const RegexMatchExpression &rhs );
    RegexMatchExpression &operator=( const RegexMatchExpression &rhs );
};

#endif /* REGEXMATCHEXPRESSION_H */
