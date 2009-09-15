#ident "file_id $Id: StubDelimitedSource.cpp,v 1.1 2005/01/04 06:45:34 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StubDelimitedSource.cpp
// Author:    mgrosso 
// Created:   Sun Jan  2 21:50:02 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: StubDelimitedSource.cpp,v 1.1 2005/01/04 06:45:34 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StubDelimitedSource.h"
#include "BaseException.h"
#define NOT_IMPLEMENTED(x) \
    PANICV( "bug: this relation has no columns, so asking for a value "\
    "is broken. column requested=%lu", x )

StubDelimitedSource::StubDelimitedSource()
{
    ;
}

StubDelimitedSource::~StubDelimitedSource()
{
    ;
}

size_t
StubDelimitedSource::getNumColumns()const
{
    return 0;
}

const char *
StubDelimitedSource::getRaw( size_t index )
{
    NOT_IMPLEMENTED(index);
    //not reached.
    return NULL;
}

char_ptr
StubDelimitedSource::getCharPtr( size_t index ) const
{
    NOT_IMPLEMENTED(index);
    //not reached.
    char_ptr x;
    return x;
}

bool
StubDelimitedSource::getBool( size_t index ) const
{
    NOT_IMPLEMENTED(index);
    //not reached.
    return false;
}

long
StubDelimitedSource::getLong( size_t index ) const
{
    NOT_IMPLEMENTED(index);
    //not reached.
    return 0;
}

long long
StubDelimitedSource::getLongLong( size_t index ) const
{
    NOT_IMPLEMENTED(index);
    //not reached.
    return 0;
}

double
StubDelimitedSource::getDouble( size_t index ) const
{
    NOT_IMPLEMENTED(index);
    //not reached.
    return 0;
}

long double
StubDelimitedSource::getLongDouble( size_t index ) const
{
    NOT_IMPLEMENTED(index);
    //not reached.
    return 0;
}

