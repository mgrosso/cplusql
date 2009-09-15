#ident "file_id $Id: RawExpression.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RawExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  8 20:27:28 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: RawExpression.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RawExpression.h"
#include "Convert.h"
#include "ptr_strdup.h"

    
RawExpression::RawExpression()
{
    ;//noop
}

RawExpression::~RawExpression()
{
    ;//noop
}

bool                
RawExpression::isNull() const
{
    return Convert::isNull( getRaw() );
}

char_ptr
RawExpression::getCharPtr() const
{
	return ptr_strdup( getRaw() );
}

bool                
RawExpression::getBool() const
{
    return Convert::convertToBool( getRaw() );
}

long int
RawExpression::getLong() const
{
    return Convert::convertToLong( getRaw() );
}

double              
RawExpression::getDouble() const
{
    return Convert::convertToDouble( getRaw() );
}

long long
RawExpression::getLongLong() const
{
    return Convert::convertToLongLong( getRaw());
}

long double
RawExpression::getLongDouble() const
{
    return Convert::convertToLongDouble( getRaw());
}

