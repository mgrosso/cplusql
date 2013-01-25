#ident "file_id $Id: DoubleProjection.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DoubleProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec 26 19:26:51 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DoubleProjection.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "DoubleProjection.h"
#include "BaseException.h"

//dummy string 32 characters long. this is sufficient length to
//represent all 64 bit numbers.
const char * const
DoubleProjection::dummybytes = "0123456789112345678921234567893101234567891123456789212345678931" ;
const size_t DoubleProjection::buflen = 64;

DoubleProjection::DoubleProjection()
:result_(0), resultc_( strdup_wnew( dummybytes ))
{
    makeString();
}

DoubleProjection::~DoubleProjection()
{
}

bool                
DoubleProjection::isNull() const
{
    return false;
}

void
DoubleProjection::notify()
{
    childNotify();
    makeString();
}

void
DoubleProjection::makeString()
{
    memset( resultc_.get(),'\0', buflen );
    snprintf( resultc_.get(),  buflen-1 , "%Lg", result_ );
}

const char *        
DoubleProjection::getRaw() 
{
    return resultc_.get() ;
}

char_ptr            
DoubleProjection::getCharPtr() const
{
    //we must return a copy, because we modify resultc in the next loadRow().
    return ptr_strdup( resultc_.get());
}

bool                
DoubleProjection::getBool() const
{
    return result_ ? true : false ;
}

long int            
DoubleProjection::getLong() const
{
    return static_cast<long int>(result_ );
}

double              
DoubleProjection::getDouble() const
{
    return static_cast<double>(result_ );
}

long long           
DoubleProjection::getLongLong() const
{
    return static_cast<long long>(result_) ;
}

long double           
DoubleProjection::getLongDouble() const
{
    return result_ ;
}

