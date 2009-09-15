#ident "file_id $Id: AbstractKeyValMap.cpp,v 1.2 2004/01/20 19:52:42 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractKeyValMap.cpp
// Author:    djain 
// Created:   Fri Dec 19 10:36:22 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractKeyValMap.cpp,v 1.2 2004/01/20 19:52:42 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AbstractKeyValMap.h"

class AbstractKeyValMapPriv
{
    private:
    friend class AbstractKeyValMap ;
    AbstractKeyValMapPriv( bool leak )
    :leak_(leak)
    {
        //noop
    }
	bool leak_;
};

AbstractKeyValMap::AbstractKeyValMap(bool leak)
{
	d_ = new AbstractKeyValMapPriv(leak);
}

AbstractKeyValMap::~AbstractKeyValMap()
{
	delete d_;
}

const char *
AbstractKeyValMap::get(ExpressionList & list, ExpressionPtr & dflt) 
{
	const char * ret = get(list);
	if(ret == NULL)
	{
		dflt->notify();
		return dflt->getRaw();
	}
	return ret;
}

const char *
AbstractKeyValMap::get(ExpressionList & list, const char * dflt)
{
	const char * ret = get(list);
	if(ret == NULL)
	{
		return dflt;
	}
	return ret;
}

const char *
AbstractKeyValMap::get(ExpressionPtr & key, ExpressionPtr & dflt)
{
	key->notify();
	const char * ret = get(key->getRaw());
	if(ret == NULL)
	{
		dflt->notify();
		return dflt->getRaw();
	}
	return ret;
}

const char *
AbstractKeyValMap::get(ExpressionPtr & key, const char * dflt)
{
	key->notify();
	const char * ret = get(key->getRaw());
	if(ret == NULL)
	{
		return dflt;
	}
	return ret;
}

bool
AbstractKeyValMap::exists(ExpressionPtr & key)
{
	key->notify();
	return exists(key->getRaw());
}

void
AbstractKeyValMap::setLeakOnDelete(bool leak)
{
	d_->leak_ = leak;
}

bool
AbstractKeyValMap::getLeakOnDelete()
{
	return d_->leak_;
}

void
AbstractKeyValMap::clear()
{
	//noop 
}
