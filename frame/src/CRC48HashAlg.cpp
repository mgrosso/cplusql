#ident "file_id $Id: CRC48HashAlg.cpp,v 1.5 2004/05/07 06:45:26 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CRC48HashAlg.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 24 11:13:57 EDT 2000 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: CRC48HashAlg.cpp,v 1.5 2004/05/07 06:45:26 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "CRC48HashAlg.h"
#include "ptr_strdup.h"
#include <string>
#include <string.h>

//long long CRC48HashAlg::crc48table [256] ;
CRC48HashAlg *CRC48HashAlg::instance_ ;

CRC48HashAlg *
CRC48HashAlg::getInstance()
{
    if ( ! instance_ )
    {
        instance_=new CRC48HashAlg();
    }
    return instance_;
}

CRC48HashAlg::~CRC48HashAlg()
{
  delete instance_;
}

CRC48HashAlg::CRC48HashAlg()
{
    //our job here is to build the crc lookup table.
    //note that the 2^48 bit is implicit in the alg.
    const long long poly48 = 
        (long long)2^36+
        (long long)2^35+
        (long long)2^23+
        (long long)2^21+
        (long long)2^15+
        (long long)2^13+
        (long long)2^8+
        (long long)2^2+
        (long long)2^0 ;
    const long long topbit = (long long)1 << ( CRCBITS - 1 ) ;
    long long r = 0;
    //fprintf(stderr, "hello\n" );
    
    for( long long i=0; i<256; i++ )
    {
        //fprintf(stderr, "hello\n" );
        r= i << ( CRCBITS - BYTEBITS );
        for( int j=0; j<8; j++)
        {
            if( r & topbit )
                r = ( r << 1 ) ^ poly48 ;
            else 
                r = ( r << 1 );
        }
        crc48table[i] = r & LOWER48 ;
        //fprintf(stderr, "%llX\n", crc48table[i] );
    }
}


long long
CRC48HashAlg::getCRC48(unsigned char *s) const
{
	long long retVal = 0;

    if (s != (unsigned char *) 0)
	{
		size_t len = strlen((const char *) s);

		retVal = getCRC48(s, len);
	}

	return retVal;
}


long long
CRC48HashAlg::getCRC48(unsigned char *s, size_t len) const
{
	unsigned char t;
	long long r= LOWER48; 

	while (len-- > 0) 
	{ 
		//excessive casting here, so we dont lose the bits 33-48 anywhere
		r = ((long long)(r << 8)) | ((long long)*s++);    //get next data byte into r
		r &= LOWER48 ;                          //zero out bits 49+
		t = (r >> ( CRCBITS - BYTEBITS)) & 0xFF;//get high byte
		r ^= crc48table[t];                     //munge r accordingly
	} 

	return ((r ^ LOWER48) + OFFSET_49_TH_BIT) ;
}


/*
long long
CRC48HashAlg::getCRC48Slowly( unsigned char *s )
{
    unsigned char c;
    while(c=*s++)
    {
        for( int i=0; i<8; ++i )
        {
	}
   }
}

long long
CRC48HashAlg::reflect( long long ref, unsigned int bitwidth )
{
    //swap bits 0,7, 6 and 1 etc...
    long long value ;
    for(int i = 1; i < (bitwidth + 1); i++) 
    { 
        if(ref & 1) 
              value |= 1 << (bitwidth - i); 
        ref >>= 1; 
    } 
    return value; 
}
*/

long long
CRC48HashAlg::hash( const char *s ) const
{
	return getCRC48((unsigned char *)s);
}

long long
CRC48HashAlg::hash( const long long int &ll ) const
{
    char_ptr lcp=ptr_strdup_longlong( ll );
    return hash( lcp.get() );
}

long long
CRC48HashAlg::hash( long l ) const
{
    char_ptr lcp=ptr_strdup_long( l );
    return hash( lcp.get() );
}

long long
CRC48HashAlg::hash( double d ) const
{
    char_ptr lcp=ptr_strdup_double( d );
    return hash( lcp.get() );
}

long long
CRC48HashAlg::getNumBuckets() const
{
    return MAX_48_BIT_CRC ;
}

