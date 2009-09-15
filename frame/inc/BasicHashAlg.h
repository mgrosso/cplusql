// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BasicHashAlg.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 19 19:20:08 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   provides a basic implementation of the AbstractHashAlg, with
//              a focus on providing a fairly even hash bucket distribution
//              as often as possible rather than on performance.
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: BasicHashAlg.h,v 1.2 2003/06/10 01:54:40 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef BASICHASHALG_H
#define BASICHASHALG_H 1

#include "AbstractHashAlg.h"
#include <limits.h>


class BasicHashAlg  : public AbstractHashAlg
{
    public:
        BasicHashAlg( long long num_buckets, unsigned int magic=47 );
        ~BasicHashAlg();
        virtual long long hash( const char *s ) const ;
        virtual long long hash( const long long int &ll ) const ;
        virtual long long hash( long l ) const ;
        virtual long long hash( double d) const ;
        virtual long long getNumBuckets() const ;

        static long long getHash( const char *s, 
                                        long long buckets=UINT_MAX ) ;
        static long long getHash( const long long int &ll, 
                                        long long buckets=UINT_MAX ) ;
        static long long getHash( long l, 
                                        long long buckets=UINT_MAX ) ;
        static long long getHash( double d, 
                                        long long buckets=UINT_MAX) ;
    private:
        long long num_buckets_ ;
        unsigned int magic_ ;

    private:
        BasicHashAlg( const BasicHashAlg &rhs );
        BasicHashAlg & operator= ( const BasicHashAlg &rhs );
};

#endif /* BASICHASHALG_H */
