#ident "file_id $Id: FloorHashAlg.cpp,v 1.1 2004/05/05 23:02:02 bdonalds Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FloorHashAlg.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 24 11:13:57 EDT 2000 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: FloorHashAlg.cpp,v 1.1 2004/05/05 23:02:02 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "FloorHashAlg.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include <string>
#include <cmath>

FloorHashAlg *FloorHashAlg::instance_ ;

FloorHashAlg *
FloorHashAlg::getInstance()
{
    if ( ! instance_ )
    {
        instance_=new FloorHashAlg();
    }
    return instance_;
}

FloorHashAlg::~FloorHashAlg()
{
  delete instance_;
}

FloorHashAlg::FloorHashAlg()
{
}

long long
FloorHashAlg::getFloorHash( unsigned char *s ) const
{
    if(!s)
        return (long long)0;

    //cast s to long long
    return (long long)floor( Convert::convertToDouble( (const char *)s ));
}


long long
FloorHashAlg::hash( const char *s ) const
{
	return getFloorHash((unsigned char *)s);
}

long long
FloorHashAlg::hash( const long long int &ll ) const
{
    char_ptr lcp=ptr_strdup_longlong( ll );
    return hash( lcp.get() );
}

long long
FloorHashAlg::hash( long l ) const
{
    char_ptr lcp=ptr_strdup_long( l );
    return hash( lcp.get() );
}

long long
FloorHashAlg::hash( double d ) const
{
    char_ptr lcp=ptr_strdup_double( d );
    return hash( lcp.get() );
}

long long
FloorHashAlg::getNumBuckets() const
{
    return 0 ;
}

