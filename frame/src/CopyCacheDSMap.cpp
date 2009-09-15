#ident "file_id $Id: CopyCacheDSMap.cpp,v 1.5 2004/09/21 00:13:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CopyCacheDSMap.cpp
// Author:    mgrosso 
// Created:   Fri Mar 28 14:52:48 EST 2003 on dhcp-172-18-100-121.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CopyCacheDSMap.cpp,v 1.5 2004/09/21 00:13:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CopyCacheDSMap.h"
#include "BaseException.h"
#include <map>
#include "ptr_strdup.h"
#include "hashFuncs.h"

typedef map< const char *, DelimitedSourcePtr, cc_less_than > dsmap_t;

class CopyCacheDSMapPriv 
{
    private:
    friend class CopyCacheDSMap ;
    CopyCacheDSMapPriv(AbstractDelimitedSourceMapPtr & adsm)
	: adsm_(adsm), max_size_(0)
    {
        //TODO
    };
	char_ptr_list list_;
	AbstractDelimitedSourceMapPtr adsm_;
    dsmap_t m_;
	size_t max_size_;
};

CopyCacheDSMap::CopyCacheDSMap(AbstractDelimitedSourceMapPtr & adsm)
{
    d_=new CopyCacheDSMapPriv (adsm);
}

CopyCacheDSMap::~CopyCacheDSMap()
{
    delete d_;
}

//the first element in the list is the key into the map
DelimitedSourcePtr
CopyCacheDSMap::get_( ExpressionList & list )
{
    ExpressionListItr elb;
    ExpressionListItr ele;
	char_ptr_list l;
    for( elb=list->begin(), ele=list->end(); elb!=ele ; ++elb )
    {
		l.push_back((*elb)->getCharPtr());
	}
	
	char_ptr keyp = ptr_strdup_ptr_list(l, "|", false);
	const char * key = keyp.get();
    dsmap_t::iterator i=d_->m_.find( key );
    dsmap_t::iterator e=d_->m_.end();
    if( i==e )
    {
		clearStale();
		d_->list_.push_back(keyp);
        DelimitedSourcePtr ds(d_->adsm_->get(list));
        d_->m_.insert( pair< const char *, DelimitedSourcePtr >(key,ds));
		return ds;
    }
	else
	{
    	return (*i).second ;
    }
}


DelimitedSourcePtr
CopyCacheDSMap::get_( const char *key )
{
    dsmap_t::iterator i=d_->m_.find( key );
    dsmap_t::iterator e=d_->m_.end();
    if( i==e )
    {
		clearStale();
		char_ptr f = ptr_strdup(key);
		d_->list_.push_back(f);
        DelimitedSourcePtr ds(d_->adsm_->get(f.get()));
        d_->m_.insert( pair< const char *, DelimitedSourcePtr >(f.get(),ds));
		return ds;
    }
	else
	{
    	return (*i).second ;
    }
}

bool
CopyCacheDSMap::exists_( ExpressionList & list)
{
	try
	{
		get_( list );
		return true;
	}catch(BaseException & e){
		return false;
	}
}

bool
CopyCacheDSMap::exists_( const char * key )
{
	try
	{
		get_( key );
		return true;
	}catch(BaseException & e){
		return false;
	}
}

void
CopyCacheDSMap::setCacheSize( int size )
{
	d_->max_size_ = size;
}

int
CopyCacheDSMap::getCacheSize() const
{
	return d_->max_size_;
}

void
CopyCacheDSMap::clearStale()
{
	if(d_->max_size_ > 0 && d_->list_.size() == d_->max_size_ )
	{
		char_ptr_list::iterator beg = d_->list_.begin();
		char_ptr c(ptr_strdup((*beg).get()));
		d_->list_.pop_front();
		dsmap_t::iterator f = d_->m_.find(c.get());
		dsmap_t::iterator e = d_->m_.end();
		if(f != e)
		{
			d_->m_.erase(f);
		}
	}
}

size_t
CopyCacheDSMap::size_()
{
	return d_->m_.size();
}

const char *
CopyCacheDSMap::getRelationName()const
{
	return d_->adsm_->getRelationName();
}

char_ptr
CopyCacheDSMap::getRelationHeader()const
{
	return d_->adsm_->getRelationHeader();
}

size_t
CopyCacheDSMap::getNumColumns()const
{
	return d_->adsm_->getNumColumns();
}

const char *
CopyCacheDSMap::getColumnName( size_t index )const
{
	return d_->adsm_->getColumnName(index);
}

size_t
CopyCacheDSMap::getColumnIndex( const char *col_name )const
{
	return d_->adsm_->getColumnIndex(col_name);
}
