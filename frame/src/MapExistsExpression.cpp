#ident "file_id $Id: MapExistsExpression.cpp,v 1.8 2004/01/22 18:28:08 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MapExistsExpression.cpp
// Author:    mgrosso 
// Created:   Mon Mar 31 01:15:02 EST 2003 on caliban.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: MapExistsExpression.cpp,v 1.8 2004/01/22 18:28:08 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MapExistsExpression.h"
#include "AbstractKeyValMap.h"

    

class MapExistsExpressionPriv 
{
    private:
    friend class MapExistsExpression ;
    MapExistsExpressionPriv(
        AbstractKeyValMapPtr &map,
        ExpressionPtr &key
    ):map_(map),key_(key)
    {
        key_list_=false;
    };
    MapExistsExpressionPriv(
        AbstractKeyValMapPtr &map,
        ExpressionList &list
    ):map_(map),list_(list)
    {
        key_list_=true;
    };

    AbstractKeyValMapPtr map_ ;
    ExpressionPtr key_ ;
    ExpressionList list_ ;
    char_ptr        raw_;
	bool key_list_;
};

MapExistsExpression::MapExistsExpression(
        AbstractKeyValMapPtr &map,
        ExpressionPtr &key
)
{
    d_=new MapExistsExpressionPriv (map, key );
}

MapExistsExpression::MapExistsExpression(
        AbstractKeyValMapPtr &map,
        ExpressionList &list
)
{
    d_=new MapExistsExpressionPriv (map, list );
}

MapExistsExpression::~MapExistsExpression()
{
    delete d_;
}

void
MapExistsExpression::notify()
{
	if(!d_->key_list_)
	{
    	d_->key_->notify();
	}
	else
	{
   	    ExpressionListItr b;
	    ExpressionListItr e;
    	for( b=d_->list_->begin(), e=d_->list_->end(); b!=e ; ++b )
    	{
       		(*b)->notify();
    	}
	}
}

bool
MapExistsExpression::isNull() const
{
    return getBool() ? false : true ;
}

bool                
MapExistsExpression::getBool() const
{
	if(!d_->key_list_)
	{
    	return d_->map_->exists( d_->key_ );
	}
	else
	{
    	return d_->map_->exists( d_->list_ );
	}
}


