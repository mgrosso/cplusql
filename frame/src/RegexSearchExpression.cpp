#ident "file_id $Id: RegexSearchExpression.cpp,v 1.2 2004/08/03 23:37:37 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexSearchExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 19:51:27 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: RegexSearchExpression.cpp,v 1.2 2004/08/03 23:37:37 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RegexSearchExpression.h"

    

class RegexSearchExpressionPriv 
{
    private:
    friend class RegexSearchExpression ;
    RegexSearchExpressionPriv()
    {
        //TODO
    };
};

RegexSearchExpression::RegexSearchExpression()
{
    d_=new RegexSearchExpressionPriv ();
}

RegexSearchExpression::~RegexSearchExpression()
{
    delete d_;
}

bool
RegexSearchExpression::isNull() const
{
    return false;
}

bool
RegexSearchExpression::getBool() const
{
    return false;
}
