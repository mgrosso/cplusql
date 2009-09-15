#ident "file_id $Id: ProxyExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyExpression.cpp
// Author:    mgrosso 
// Created:   Mon Mar 31 05:54:20 EST 2003 on caliban.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ProxyExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ProxyExpression.h"

ProxyExpression::ProxyExpression()
{
}

ProxyExpression::~ProxyExpression()
{
}

bool                
ProxyExpression::isNull() const 
{
    return getProxy()->isNull();
}

const char *        
ProxyExpression::getRaw()
{
    return getProxy()->getRaw();
}

char_ptr            
ProxyExpression::getCharPtr() const 
{
    return getProxy()->getCharPtr();
}

bool                
ProxyExpression::getBool() const 
{
    return getProxy()->getBool();
}

long int            
ProxyExpression::getLong() const
{
    return getProxy()->getLong();
}

double              
ProxyExpression::getDouble() const
{
    return getProxy()->getDouble();
}

long long           
ProxyExpression::getLongLong() const
{
    return getProxy()->getLongLong();
}

long double           
ProxyExpression::getLongDouble() const
{
    return getProxy()->getLongDouble();
}


