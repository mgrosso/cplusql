#ident "file_id $Id: BooleanExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BooleanExpression.cpp
// Author:    mgrosso 
// Created:   Mon Mar 31 01:15:02 EST 2003 on caliban.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: BooleanExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BooleanExpression.h"
#include "ptr_strdup.h"

    
const char *const BooleanExpression::false_raw = "0";
const char *const BooleanExpression::true_raw = "1";
char_ptr BooleanExpression::false_rawp = strdup_wnew( BooleanExpression::false_raw );
char_ptr BooleanExpression::true_rawp = strdup_wnew( BooleanExpression::true_raw );



BooleanExpression::BooleanExpression()
{
    ;//noop
}

BooleanExpression::~BooleanExpression()
{
    ;//noop
}

const char *
BooleanExpression::getRaw()
{
    return getBool() ? true_raw : false_raw ;
}

char_ptr
BooleanExpression::getCharPtr() const
{
    return getBool() ? true_rawp : false_rawp ;
}

long int
BooleanExpression::getLong() const
{
    return static_cast<long int>(getBool());
}

double              
BooleanExpression::getDouble() const
{
    return static_cast<double>(getBool());
}

long long
BooleanExpression::getLongLong() const
{
    return static_cast<long long>(getBool());
}

long double
BooleanExpression::getLongDouble() const
{
    return static_cast<long double>(getBool());
}

