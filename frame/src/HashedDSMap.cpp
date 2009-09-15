#ident "file_id $Id: HashedDSMap.cpp,v 1.3 2004/09/21 00:14:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashedDSMap.cpp
// Author:    djain 
// Created:   Sun Apr 27 18:15:05 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashedDSMap.cpp,v 1.3 2004/09/21 00:14:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "HashedDSMap.h"
#include "HashConnections.h"
#include "BasicHashAlg.h"
#include "ptr_strdup.h"
#include "BaseException.h"
#include "RelationMeta.h"

typedef map<const char*, AbstractDelimitedSourceMapPtr, cc_less_than> ads_map_t;

class HashedDSMapPriv 
{
    private:
    friend class HashedDSMap ;

	HashConnectionsPtr conn_hash_;
	RelationMetaPtr rel_;
	int buckets_;
	HashAlgPtr alg_;
	ADSMapFactoryPtr factory_;
	ads_map_t map_;
	char_ptr_list list_;
};

HashedDSMap::HashedDSMap( 
	RelationMetaPtr & rel,
	HashConnectionsPtr & hash,
	ADSMapFactoryPtr & factory,
	int buckets
)
{
    d_=new HashedDSMapPriv ();
	d_->conn_hash_ = hash;
	d_->rel_ = rel;
	d_->factory_ = factory;
	d_->buckets_ = buckets;
	d_->alg_ = new BasicHashAlg(d_->buckets_);
}

HashedDSMap::~HashedDSMap()
{
    delete d_;
}

//the first element in the list is the key into the map
DelimitedSourcePtr
HashedDSMap::get_( ExpressionList & list )
{
	const char * key = (*(list->begin()))->getRaw();
	char_ptr bucket = ptr_strdup_longlong(d_->alg_->hash(key));
	ads_map_t::iterator beg = d_->map_.find(bucket.get());
	AbstractDelimitedSourceMapPtr map;
	if(beg != d_->map_.end())
	{
		map = (*beg).second;
	}
	else
	{
		ConnectionPtr & conn(d_->conn_hash_->getConnection(bucket.get()));
		if(conn==NULL)
		{
			throw BaseException::factory("No connection found for bucket[%s] key[%s]", bucket.get(), key);
		}
		else
		{
			d_->list_.push_back(bucket);
			map = d_->factory_->createMap(conn, bucket.get());
			d_->map_.insert(pair<const char*, AbstractDelimitedSourceMapPtr>(bucket.get(),map));
		}
	}
	return map->get(list);
}
DelimitedSourcePtr
HashedDSMap::get_( const char * key )
{
	char_ptr bucket = ptr_strdup_longlong(d_->alg_->hash(key));
	ads_map_t::iterator beg = d_->map_.find(bucket.get());
	AbstractDelimitedSourceMapPtr map;
	if(beg != d_->map_.end())
	{
		map = (*beg).second;
	}
	else
	{
		ConnectionPtr & conn(d_->conn_hash_->getConnection(bucket.get()));
		if(conn==NULL)
		{
			throw BaseException::factory("No connection found for bucket[%s] key[%s]", bucket.get(), key);
		}
		else
		{
			d_->list_.push_back(bucket);
			map = d_->factory_->createMap(conn, bucket.get());
			d_->map_.insert(pair<const char*, AbstractDelimitedSourceMapPtr>(bucket.get(),map));
		}
	}
	return map->get(key);
}

bool
HashedDSMap::exists_( const char * key )
{
	try
	{
		get_(key);
		return true;
	}catch(BaseException & e) {
		return false;
	}
}

bool
HashedDSMap::exists_( ExpressionList & list )
{
	try
	{
		get_(list);
		return true;
	}catch(BaseException & e) {
		return false;
	}
}

size_t
HashedDSMap::size_()
{
	return d_->buckets_;
}

const char *
HashedDSMap::getRelationName()const
{
	return d_->rel_->getRelationName();
}

size_t
HashedDSMap::getNumColumns()const
{
	return d_->rel_->getNumColumns();
}

const char *
HashedDSMap::getColumnName( size_t index )const
{
	return d_->rel_->getColumnName(index);
}

size_t
HashedDSMap::getColumnIndex( const char *col_name )const
{
	return d_->rel_->getColumnIndex(col_name);
}

char_ptr
HashedDSMap::getRelationHeader()const
{
	return d_->rel_->getRelationHeader();
}
