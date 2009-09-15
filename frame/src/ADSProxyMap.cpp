#ident "file_id $Id: ADSProxyMap.cpp,v 1.2 2004/01/20 19:52:42 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ADSProxyMap.cpp
// Author:    djain 
// Created:   Fri Dec 19 10:55:25 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ADSProxyMap.cpp,v 1.2 2004/01/20 19:52:42 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ADSProxyMap.h"
#include "AbstractDelimitedSource.h"

class ADSProxyMapPriv 
{
    private:
    friend class ADSProxyMap ;
    ADSProxyMapPriv( AbstractDelimitedSourceMapPtr & map, int index )
	:map_(map), index_(index)
    {
        //noop
    };
	AbstractDelimitedSourceMapPtr & map_;
	int index_;
};

ADSProxyMap::ADSProxyMap(AbstractDelimitedSourceMapPtr & map, int index)
{
    d_=new ADSProxyMapPriv (map, index);
}

ADSProxyMap::~ADSProxyMap()
{
    delete d_;
}

const char *
ADSProxyMap::get( ExpressionList & list )
{
    DelimitedSourcePtr src(d_->map_->get( list ));
	if(src != NULL)
	{
    	return src->getRaw(d_->index_);
	}
	else
	{
		return NULL;
	}
}

const char *
ADSProxyMap::get( const char *key )
{
    DelimitedSourcePtr src(d_->map_->get( key ));
	if(src != NULL)
	{
    	return src->getRaw(d_->index_);
	}
	else
	{
		return NULL;
	}
}

bool
ADSProxyMap::exists( ExpressionList & list )
{
    return d_->map_->exists( list );
}

bool
ADSProxyMap::exists( const char * key )
{
    return d_->map_->exists( key );
}

size_t
ADSProxyMap::size() const
{
    return d_->map_->size();
}
