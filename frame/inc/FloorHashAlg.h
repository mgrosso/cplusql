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
// $Id: FloorHashAlg.h,v 1.1 2004/05/05 23:03:14 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef FLOORHASHALG_H
#define FLOOR_HASHALGH 1

#include "AbstractHashAlg.h"

class FloorHashAlg : public AbstractHashAlg 
{
    public:
    //singleton
    static FloorHashAlg *getInstance();

    long long getFloorHash( unsigned char *s ) const;
    inline long long getFloorHash(const char *s ) const
        { return getFloorHash( (unsigned char *) s );};

    long long hash(const char *s ) const;
    long long hash(const long long int &ll ) const;
    long long hash( long l ) const;
    long long hash( double d ) const;
    long long getNumBuckets() const;

    ~FloorHashAlg();

    private:
    //singleton instance 
    static FloorHashAlg *instance_;

    //not implemented
    FloorHashAlg();
    FloorHashAlg( const FloorHashAlg &rhs );
    const FloorHashAlg &operator=( const FloorHashAlg &rhs );

    //helper function
    long long reflect( long long ref, unsigned int bitwidth );

};

#endif /* FLOORHASHALG_H */
