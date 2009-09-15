// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractHashAlg.h
// Author:    mgrosso Matthew Grosso
// Created:   Thu Jan 16 04:33:18 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   provide implementation of basic hash for various data types
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: AbstractHashAlg.h,v 1.3 2004/01/16 21:11:22 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACTHASHALG_H
#define ABSTRACTHASHALG_H 1

#include "ptr.h"

class AbstractHashAlg 
{
    protected:
        AbstractHashAlg();
    public:
        virtual ~AbstractHashAlg();
        //each of the hash() functions should return a number between 0 
        //and getNumBuckets()-1.
        virtual long long hash( const char *s ) const =0 ;
        virtual long long hash( const long long int &ll ) const =0 ;
        virtual long long hash( long l ) const =0 ;
        virtual long long hash( double d ) const =0 ;
        virtual long long getNumBuckets() const =0 ;
    private:
        AbstractHashAlg( const AbstractHashAlg &rhs );
        AbstractHashAlg & operator= ( const AbstractHashAlg &rhs );
};

typedef ptr<AbstractHashAlg> HashAlgPtr;

#endif /* ABSTRACTHASHALG_H */

