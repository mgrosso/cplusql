#ident "file_id $Id: StringProjection.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringProjection.cpp
// Author:    mgrosso 
// Created:   Tue Apr 15 06:41:20 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: StringProjection.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StringProjection.h"
#include "Convert.h"

class StringProjectionPriv 
{
    private:
    friend class StringProjection ;
    StringProjectionPriv()
    :cp_(ptr_strdup(""))
    {
        //noop
    };
    char_ptr cp_;
};

StringProjection::StringProjection()
{
    d_=new StringProjectionPriv ();
}

StringProjection::~StringProjection()
{
    delete d_;
}

void                
StringProjection::set( char_ptr &cp ) 
{
    d_->cp_ = cp ;
}

void                
StringProjection::set( char_ptr cp ) 
{
    d_->cp_ = cp ;
}

bool                
StringProjection::isNull() const
{
    return 0==*d_->cp_.get();
}

void
StringProjection::notify()
{
    childNotify();
}

const char *        
StringProjection::getRaw() 
{
    return d_->cp_.get() ;
}

const char *        
StringProjection::getRaw() const
{
    return d_->cp_.get() ;
}

char_ptr            
StringProjection::getCharPtr() const
{
    return d_->cp_;
}

bool                
StringProjection::getBool() const
{
    return Convert::convertToBool( getRaw());
}

long int            
StringProjection::getLong() const
{
    return Convert::convertToLong( getRaw());
}

double              
StringProjection::getDouble() const
{
    return Convert::convertToDouble( getRaw());
}

long long           
StringProjection::getLongLong() const
{
    return Convert::convertToLongLong( getRaw());
}
long double           
StringProjection::getLongDouble() const
{
    return Convert::convertToLongDouble( getRaw());
}
