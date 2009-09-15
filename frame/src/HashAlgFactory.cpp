#ident "file_id $Id: HashAlgFactory.cpp,v 1.6 2004/05/05 23:02:02 bdonalds Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashAlgFactory.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 19 20:25:07 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashAlgFactory.cpp,v 1.6 2004/05/05 23:02:02 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "HashAlgFactory.h"
#include "BasicHashAlg.h"
#include "CRC48HashAlg.h"
#include "CeilHashAlg.h"
#include "FloorHashAlg.h"
#include "Config.h"

bool HashAlgFactory::initonce = false ; 
bool HashAlgFactory::defmagic ; 
// magic is not referenced unless it has been explicitly set. 
unsigned int HashAlgFactory::magic ; 

AbstractHashAlg *
HashAlgFactory::getBasicHash( unsigned int num_buckets )
{
    if( !initonce )
    {
        defmagic=true;
        try{
            Config c;
            c.loadEnv();
            if( NULL != c.findString( IMP_HASH_MAGIC_ENV ))
            {
                magic=c.getInt( IMP_HASH_MAGIC_ENV );
                defmagic=false;
            }
        }catch(...)
        {
            ;//ignore
        }
        initonce=true;
    }
    if( defmagic )
    {
        return new BasicHashAlg( num_buckets );
    }
    return new BasicHashAlg( num_buckets, magic );
}

ptr< AbstractHashAlg > 
HashAlgFactory::getBasicHashPtr( unsigned int num_buckets )
{
    return ptr<AbstractHashAlg>((AbstractHashAlg *)getBasicHash(num_buckets));
}

AbstractHashAlg *
HashAlgFactory::getCRC48Hash()
{
	return CRC48HashAlg::getInstance();
}

ptr< AbstractHashAlg > 
HashAlgFactory::getCRC48HashPtr()
{
    return ptr<AbstractHashAlg>((AbstractHashAlg *)getCRC48Hash());
}

AbstractHashAlg *
HashAlgFactory::getCeilHash()
{
	return CeilHashAlg::getInstance();
}

ptr< AbstractHashAlg > 
HashAlgFactory::getCeilHashPtr()
{
    return ptr<AbstractHashAlg>((AbstractHashAlg *)getCeilHash());
}

AbstractHashAlg *
HashAlgFactory::getFloorHash()
{
	return FloorHashAlg::getInstance();
}

ptr< AbstractHashAlg > 
HashAlgFactory::getFloorHashPtr()
{
    return ptr<AbstractHashAlg>((AbstractHashAlg *)getFloorHash());
}
