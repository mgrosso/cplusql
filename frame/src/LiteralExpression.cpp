#ident "file_id $Id: LiteralExpression.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LiteralExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:37:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: LiteralExpression.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "LiteralExpression.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"


LiteralExpression::LiteralExpression( const char *value )
:val_(strdup_wnew(value))
{
    ;//noop
}

LiteralExpression::~LiteralExpression()
{
    delete [] val_;
}

void
LiteralExpression::set( const char *value )
{
    if(!value)
    {
        PANIC( "LiteralExpression::set( const char *value ): value is NULL\n");
    }
    delete [] val_ ;
    val_=strdup_wnew(value);
}


bool                
LiteralExpression::isNull() const
{
    return Convert::isNull( val_ );
}

const char *
LiteralExpression::getRaw()
{
    return val_ ;
}

char_ptr
LiteralExpression::getCharPtr() const
{
    return ptr_strdup_wnull( val_  );
}

bool                
LiteralExpression::getBool() const
{
    return Convert::convertToBool( val_ );
}

long int
LiteralExpression::getLong() const
{
    return Convert::convertToLong( val_);
}

double              
LiteralExpression::getDouble() const
{
    return Convert::convertToDouble( val_);
}

long long
LiteralExpression::getLongLong() const
{
    return Convert::convertToLongLong( val_);
}

long double
LiteralExpression::getLongDouble() const
{
    return Convert::convertToLongDouble( val_);
}

