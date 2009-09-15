#ident "file_id $Id: RegexMatchExpression.cpp,v 1.2 2004/05/13 23:09:41 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexMatchExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 19:05:55 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: RegexMatchExpression.cpp,v 1.2 2004/05/13 23:09:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RegexMatchExpression.h"
#include "RegexWrap.h"
#include "BaseException.h"


class RegexMatchExpressionPriv 
{
    private:
    friend class RegexMatchExpression ;
    regex_t         regex_ ;
    ExpressionPtr   operand_ ;
    bool            result_;

    RegexMatchExpressionPriv(
        ExpressionPtr &operand, 
        const char *pattern, 
        bool case_insensitive  );
    ~RegexMatchExpressionPriv();

};

RegexMatchExpressionPriv::RegexMatchExpressionPriv( 
    ExpressionPtr &operand, const char *pattern, bool case_insensitive  )
:operand_(operand), result_(false)
{
    int flags = REG_EXTENDED | 
                ( case_insensitive ? REG_ICASE : 0 ) |
                REG_NOSUB ;
    RegexWrap::init( &regex_, pattern, flags );
}

RegexMatchExpressionPriv::~RegexMatchExpressionPriv()
{
    RegexWrap::release( &regex_ );
}

RegexMatchExpression::~RegexMatchExpression()
{
    delete d_ ;
}

RegexMatchExpression::RegexMatchExpression(
    ExpressionPtr &operand, const char *pattern, bool case_insensitive  )
{
    d_=new RegexMatchExpressionPriv ( operand, pattern, case_insensitive);
}

bool
RegexMatchExpression::isNull() const 
{
    return false;
}

bool
RegexMatchExpression::getBool() const 
{
    return d_->result_ ;
}

void
RegexMatchExpression::notify()
{
    d_->operand_->notify();
    d_->result_ = RegexWrap::match( &d_->regex_, d_->operand_->getRaw()); 
}

bool
RegexMatchExpression::match( const char *operand ) const 
{
    return RegexWrap::match( &d_->regex_, operand );
}

