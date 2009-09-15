#ident "file_id $Id: NumericProjection.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NumericProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec 26 19:26:51 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: NumericProjection.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "NumericProjection.h"
#include "BaseException.h"

//dummy string 64 characters long. 
const char * const
NumericProjection::dummybytes = 
"01234567891123456789212345678931234567894123456789512345678961234" ;
const size_t NumericProjection::buflen = 64;

NumericProjection::NumericProjection()
:result_(0), resultc_( strdup_wnew( dummybytes ))
{
    makeString();
}

NumericProjection::~NumericProjection()
{
}

bool                
NumericProjection::isNull() const
{
    return false;
}

void
NumericProjection::notify()
{
    childNotify();
    makeString();
}

void
NumericProjection::makeString()
{
    memset( resultc_.get(),'\0', buflen );
    snprintf( resultc_.get(),  buflen-1 , "%.62Lg", result_ );
}

const char *        
NumericProjection::getRaw() 
{
    return resultc_.get() ;
}

char_ptr            
NumericProjection::getCharPtr() const
{
    //we must return a copy, because we modify resultc in the next loadRow().
    return ptr_strdup( resultc_.get());
}

bool                
NumericProjection::getBool() const
{
    return result_ ? true : false ;
}

long int            
NumericProjection::getLong() const
{
    return static_cast<long int>(result_ );
}

double              
NumericProjection::getDouble() const
{
    return static_cast<double>(result_ );
}

long long           
NumericProjection::getLongLong() const
{
    return static_cast<long long >(result_ );
}

long double           
NumericProjection::getLongDouble() const
{
    return result_;
}

