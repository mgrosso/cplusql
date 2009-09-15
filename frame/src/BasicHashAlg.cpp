#ident "file_id $Id: BasicHashAlg.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BasicHashAlg.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 19 19:42:22 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: BasicHashAlg.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BasicHashAlg.h"
#include "ptr_strdup.h"

BasicHashAlg::BasicHashAlg( long long num_buckets, unsigned int magic )
:num_buckets_(num_buckets),magic_(magic)
{
    ;
}

BasicHashAlg::~BasicHashAlg()
{
    ;
}

long long
BasicHashAlg::hash( const char *s ) const 
{
    long long ret=0;
    unsigned int i=0;
    const char *b=s;
    while(*b)
    {
        ret += ((i++ + magic_ ) * (*b++));
    }
    return ret % num_buckets_ ;
}

long long 
BasicHashAlg::hash( const long long int &ll ) const 
{
    char_ptr lcp=ptr_strdup_longlong( ll );
    return hash( lcp.get() );
}

long long 
BasicHashAlg::hash( long l ) const 
{
    char_ptr lcp=ptr_strdup_long( l );
    return hash( lcp.get() );
}

long long 
BasicHashAlg::hash( double d ) const 
{
    char_ptr lcp=ptr_strdup_double( d );
    return hash( lcp.get() );
}

long long 
BasicHashAlg::getNumBuckets() const 
{
    return num_buckets_ ;
}

//TODO: stupid to have two different implementations here. I dont want
//to force 47 as magic on objects though. 
long long 
BasicHashAlg::getHash( const char *s, long long buckets ) 
{
    long long ret=0;
    unsigned int i=0;
    const char *b=s;
    while(*b)
    {
        ret += ((i++ + 47 ) * (*b++));
    }
    return ret % buckets ;
}

long long 
BasicHashAlg::getHash( const long long int &ll, long long buckets ) 
{
    char_ptr lcp=ptr_strdup_longlong( ll );
    return getHash( lcp.get(), buckets );
}

long long 
BasicHashAlg::getHash( long l, long long buckets ) 
{
    char_ptr lcp=ptr_strdup_long( l );
    return getHash( lcp.get(), buckets );
}

long long 
BasicHashAlg::getHash( double d, long long buckets ) 
{
    char_ptr lcp=ptr_strdup_double( d );
    return getHash( lcp.get(), buckets );
}


