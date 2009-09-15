// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Line:      HashedDSMap.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Aug 11 20:42:55 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: HashedDSMap.h,v 1.3 2004/09/21 00:14:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef HASHEDDSMAP_H
#define HASHEDDSMAP_H 1

#include "AbstractHashAlg.h"
#include "AbstractDelimitedSourceMap.h"
#include "ADSMapFactory.h"
#include "HashConnections.h"
#include "RelationMeta.h"

class HashedDSMapPriv ;

class HashedDSMap : public AbstractDelimitedSourceMap
{
	protected:
    virtual DelimitedSourcePtr get_( const char * key );
    virtual DelimitedSourcePtr get_( ExpressionList & exp );
    virtual bool exists_( const char * key );
    virtual bool exists_( ExpressionList & list );
    virtual size_t size_();

    public:
    HashedDSMap( RelationMetaPtr & rel, HashConnectionsPtr & conn, ADSMapFactoryPtr & factory, int num_buckets );
    virtual                     ~HashedDSMap();

    //extensions
    virtual const char *    getRelationName()const;
    virtual size_t          getNumColumns()const;
    virtual const char *    getColumnName( size_t index )const;
    virtual size_t          getColumnIndex( const char *col_name )const;
    virtual char_ptr        getRelationHeader()const;

    private:
    HashedDSMapPriv *d_ ;

    //not implemented
    HashedDSMap();
    HashedDSMap( const HashedDSMap &rhs);
    HashedDSMap &operator=( const HashedDSMap &rhs);
};

#endif /* HASHEDDSMAP_H */
