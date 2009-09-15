#ident "file_id $Id: CharExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CharExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:37:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: CharExpression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CharExpression.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"

CharExpression::CharExpression( const char *value )
:d_(value)
{
    ;//noop
}

CharExpression::~CharExpression()
{
    ;//noop
}

void
CharExpression::set( const char *value )
{
    d_=value;
}


bool                
CharExpression::isNull() const
{
    return Convert::isNull( d_);
}

const char *
CharExpression::getRaw()
{
    return d_;
}

char_ptr
CharExpression::getCharPtr() const
{
    return ptr_strdup_wnull( d_ );
}

bool                
CharExpression::getBool() const
{
    return Convert::convertToBool( d_);
}

long int
CharExpression::getLong() const
{
    return Convert::convertToLong( d_);
}

double              
CharExpression::getDouble() const
{
    return Convert::convertToDouble( d_);
}

long long
CharExpression::getLongLong() const
{
    return Convert::convertToLongLong( d_);
}

long double
CharExpression::getLongDouble() const
{
    return Convert::convertToLongDouble( d_);
}

