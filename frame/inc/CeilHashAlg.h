// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 24 11:13:53 EDT 2000 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: CeilHashAlg.h,v 1.1 2004/05/05 23:03:14 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CEILHASHALG_H
#define CEIL_HASHALGH 1

#include "AbstractHashAlg.h"

class CeilHashAlg : public AbstractHashAlg 
{
    public:
    //singleton
    static CeilHashAlg *getInstance();

    long long getCeilHash( unsigned char *s ) const;
    inline long long getCeilHash(const char *s ) const
        { return getCeilHash( (unsigned char *) s );};

    long long hash(const char *s ) const;
    long long hash(const long long int &ll ) const;
    long long hash( long l ) const;
    long long hash( double d ) const;
    long long getNumBuckets() const;

    //uses slower method, use for correctness testing of fast method
    //long long getCeilSlowly( unsigned char *s );

    ~CeilHashAlg();

    private:
    //singleton instance 
    static CeilHashAlg *instance_;

    //not implemented
    CeilHashAlg();
    CeilHashAlg( const CeilHashAlg &rhs );
    const CeilHashAlg &operator=( const CeilHashAlg &rhs );

    //helper function
    long long reflect( long long ref, unsigned int bitwidth );

};

#endif /* CEILHASHALG_H */
