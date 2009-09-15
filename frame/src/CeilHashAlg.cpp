#ident "file_id $Id: CeilHashAlg.cpp,v 1.2 2004/06/02 21:15:05 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CeilHashAlg.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 24 11:13:57 EDT 2000 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: CeilHashAlg.cpp,v 1.2 2004/06/02 21:15:05 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "CeilHashAlg.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include <string>
#include <cmath>

CeilHashAlg *CeilHashAlg::instance_ ;

CeilHashAlg *
CeilHashAlg::getInstance()
{
    if ( ! instance_ )
    {
        instance_=new CeilHashAlg();
    }
    return instance_;
}

CeilHashAlg::~CeilHashAlg()
{
  delete instance_;
}

CeilHashAlg::CeilHashAlg()
{
}

long long
CeilHashAlg::getCeilHash( unsigned char *s ) const
{
    if(!s) return 0;
    return static_cast< long long >(
        ceil( Convert::convertToDouble((const char *)s)));
}


long long
CeilHashAlg::hash( const char *s ) const
{
	return getCeilHash((unsigned char *)s);
}

long long
CeilHashAlg::hash( const long long int &ll ) const
{
    char_ptr lcp=ptr_strdup_longlong( ll );
    return hash( lcp.get() );
}

long long
CeilHashAlg::hash( long l ) const
{
    char_ptr lcp=ptr_strdup_long( l );
    return hash( lcp.get() );
}

long long
CeilHashAlg::hash( double d ) const
{
    char_ptr lcp=ptr_strdup_double( d );
    return hash( lcp.get() );
}

long long
CeilHashAlg::getNumBuckets() const
{
    return 0 ;
}

