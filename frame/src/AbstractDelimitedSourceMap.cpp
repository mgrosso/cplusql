#ident "file_id $Id: AbstractDelimitedSourceMap.cpp,v 1.2 2004/01/20 19:52:42 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractDelimitedSourceMap.cpp
// Author:    djain 
// Created:   Fri Dec 19 13:08:44 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractDelimitedSourceMap.cpp,v 1.2 2004/01/20 19:52:42 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AbstractDelimitedSourceMap.h"
#include "Semaphore.h"

class ADSMPriv
{
	private:
	friend class AbstractDelimitedSourceMap;

	ptr<Semaphore> s_;
};

AbstractDelimitedSourceMap::AbstractDelimitedSourceMap()
{
	d_ = new ADSMPriv();
	d_->s_ = new Semaphore();
}

AbstractDelimitedSourceMap::~AbstractDelimitedSourceMap()
{
	delete d_;
}

DelimitedSourcePtr
AbstractDelimitedSourceMap::get( ExpressionList & list )
{
	d_->s_->wait();
	DelimitedSourcePtr d(get_(list));
	d_->s_->post();
	return d;
}

DelimitedSourcePtr
AbstractDelimitedSourceMap::get( ExpressionPtr & exp )
{
	return get(exp->getRaw());
}

DelimitedSourcePtr
AbstractDelimitedSourceMap::get( const char * key )
{
	d_->s_->wait();
	DelimitedSourcePtr d(get_(key));
	d_->s_->post();
	return d;
}

bool
AbstractDelimitedSourceMap::exists( ExpressionList & list)
{
	d_->s_->wait();
	bool b = exists_(list);
	d_->s_->post();
	return b;
}

bool
AbstractDelimitedSourceMap::exists( const char * key )
{
	d_->s_->wait();
	bool b = exists_(key);
	d_->s_->post();
	return b;
}

size_t
AbstractDelimitedSourceMap::size()
{
	d_->s_->wait();
	size_t s = size_();
	d_->s_->post();
	return s;
}
