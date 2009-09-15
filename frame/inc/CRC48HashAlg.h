// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      crc48.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 24 11:13:53 EDT 2000 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: CRC48HashAlg.h,v 1.3 2004/05/07 06:45:26 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CRC48HASHALG_H
#define CRC48_HASHALGH 1

#include "AbstractHashAlg.h"

class CRC48HashAlg : public AbstractHashAlg 
{
    public:
    //singleton
    static CRC48HashAlg *getInstance();

	long long getCRC48( unsigned char *s, size_t len ) const;
    long long getCRC48( unsigned char *s ) const;
    inline long long getCRC48(const char *s ) const
        { return getCRC48( (unsigned char *) s );};

    long long hash(const char *s ) const;
    long long hash(const long long int &ll ) const;
    long long hash( long l ) const;
    long long hash( double d ) const;
    long long getNumBuckets() const;

    //uses slower method, use for correctness testing of fast method
    //long long getCRC48Slowly( unsigned char *s );

    ~CRC48HashAlg();

    private:
    //singleton instance 
    static CRC48HashAlg *instance_;

    //private data
    long long crc48table [256] ;

    //constants
	static const long long MAX_48_BIT_CRC = 281474976710656LL;   // 2^48
	static const long long OFFSET_49_TH_BIT = 562949953421312LL;   // 2^49
	static const long long MAX_CRC_VALUE = 844424930131967LL; //(2^49 + (2^48 -1))
    static const long long LOWER48 = (long long)0xFFFFFFFFFFFFLL ;
    static const long long LOWBYTE = 0xFF ;
    static const int BYTEBITS = 8 ;
    static const int CRCBITS = 48 ;

    //not implemented
    CRC48HashAlg();
    CRC48HashAlg( const CRC48HashAlg &rhs );
    const CRC48HashAlg &operator=( const CRC48HashAlg &rhs );

    //helper function
    long long reflect( long long ref, unsigned int bitwidth );

};

#endif /* CRC48HASHALG_H */
