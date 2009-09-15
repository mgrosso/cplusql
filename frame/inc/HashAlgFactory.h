// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashAlgFactory.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 19 19:28:48 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashAlgFactory.h,v 1.3 2004/05/05 23:03:14 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef HASHALGFACTORY_H
#define HASHALGFACTORY_H 1

#include "AbstractHashAlg.h"
#include "ptr.h"


class HashAlgFactory {
    public:
        static AbstractHashAlg *getBasicHash( unsigned int num_buckets );
        static ptr< AbstractHashAlg > getBasicHashPtr( unsigned int num_buckets );
        static AbstractHashAlg *getCRC48Hash();
        static ptr< AbstractHashAlg > getCRC48HashPtr();
        static AbstractHashAlg *getCeilHash();
        static ptr< AbstractHashAlg > getCeilHashPtr();
        static AbstractHashAlg *getFloorHash();
        static ptr< AbstractHashAlg > getFloorHashPtr();
    private:
        static bool initonce; 
        static bool defmagic; 
        static unsigned int magic; 
};

#define IMP_HASH_MAGIC_ENV "IMP_HASH_MAGIC"

#endif /* HASHALGFACTORY_H */
