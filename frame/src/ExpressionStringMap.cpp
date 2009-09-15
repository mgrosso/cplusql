#ident "file_id $Id: ExpressionStringMap.cpp,v 1.7 2004/01/20 19:52:44 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExpressionStringMap.cpp
// Author:    mgrosso 
// Created:   Fri Mar 28 14:52:48 EST 2003 on dhcp-172-18-100-121.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ExpressionStringMap.cpp,v 1.7 2004/01/20 19:52:44 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <map>
#include <algorithm>
#include "BaseException.h"
#include "ExpressionStringMap.h"
#include "ptr_strdup.h"
#include "hashFuncs.h"

typedef map< char *, char *, cc_less_than > ccmap_t;

class ExpressionStringMapPriv 
{
    private:
    friend class ExpressionStringMap ;
    ExpressionStringMapPriv() 
    {
        //TODO
    };
    ccmap_t m_;
};

ExpressionStringMap::ExpressionStringMap()
{
    d_=new ExpressionStringMapPriv ();
	setLeakOnDelete(true);
}

ExpressionStringMap::~ExpressionStringMap()
{
    if( ! getLeakOnDelete() )
    {
        clear();
    }
    delete d_;
}


void
ExpressionStringMap::set( ExpressionPtr &key, ExpressionPtr &val )
{
    set( key->getRaw(), val->getRaw());
}

void
ExpressionStringMap::set( const char *key, const char *val )
{
    char *k=strdup_wnew( key );
    char *v=strdup_wnew( val );
    ccmap_t::iterator i=d_->m_.find( k );
    ccmap_t::iterator e=d_->m_.end();
    if( i==e )
    {
        d_->m_[k]=v;
    } else {
        delete [] (*i).second;
        (*i).second = v ;
        delete [] k;
    }
}

const char *
ExpressionStringMap::get( ExpressionList &list )
{
	if(list->size() != 1)
	{
		PANIC("The list passed into ExpressionStringMap::get( ExpressionList &list ) may only contain 1 ExpressionPtr element");
	}
    ExpressionPtr & e(*list->begin());
	e->notify();
	return get(e->getRaw());
}

const char *
ExpressionStringMap::get( const char *key )
{
    ccmap_t::iterator i=d_->m_.find( const_cast<char *>(key));
    ccmap_t::iterator e=d_->m_.end();
    if( i==e )
    {
        return NULL ;
    } 
    return (*i).second ;
}

bool
ExpressionStringMap::exists( ExpressionList &list )
{
	if(list->size() != 1)
	{
		PANIC("The list passed into ExpressionStringMap::get( ExpressionList &list ) may only contain 1 ExpressionPtr element");
	}
    ExpressionPtr & e(*list->begin());
	e->notify();
	return exists(e->getRaw());
}

bool
ExpressionStringMap::exists( const char * key )
{
    if( d_->m_.find( const_cast<char *>(key)) == d_->m_.end())
    {
        return false;
    } 
    return true;
}

struct esm_deleter
{
    public:
    inline void operator ()( ccmap_t::value_type &v )
    {
        delete [] v.first ;
        delete [] v.second ;
    };
};

void
ExpressionStringMap::clear()
{
    //why doesnt this work?
    //for_each( d_->m_.begin(), d_->m_.end(), esm_deleter );
    ccmap_t::iterator i=d_->m_.begin();
    ccmap_t::iterator e=d_->m_.end();
    for( ; i!=e; ++i )
    {
        delete [] (*i).first ;
        delete [] (*i).second ;
    }
    d_->m_.clear();
}

size_t
ExpressionStringMap::size() const
{
    return d_->m_.size();
}

