
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexSearchExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 19:51:27 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: RegexSearchExpression.h,v 1.1 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef REGEXSEARCHEXPRESSION_H
#define REGEXSEARCHEXPRESSION_H 1
    
class RegexSearchExpressionPriv ;
class RegexSearchExpression
{
    public:
    RegexSearchExpression( const char *pattern );
    virtual ~RegexSearchExpression();
    virtual bool isNull() const ;
    virtual bool getBool() const ;


    private:
    RegexSearchExpressionPriv *d_;
    //not implemented
    RegexSearchExpression();
    RegexSearchExpression( const RegexSearchExpression &rhs );
    RegexSearchExpression &operator=( const RegexSearchExpression &rhs );
};



#endif /* REGEXSEARCHEXPRESSION_H */
