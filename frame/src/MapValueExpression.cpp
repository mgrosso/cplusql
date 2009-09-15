#ident "file_id $Id: MapValueExpression.cpp,v 1.10 2004/01/21 20:17:22 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MapValueExpression.cpp
// Author:    mgrosso 
// Created:   Mon Mar 31 03:51:03 EST 2003 on caliban.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: MapValueExpression.cpp,v 1.10 2004/01/21 20:17:22 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MapValueExpression.h"
#include "CharExpression.h"

static const char *const no_init = __FILE__ ;
    

class MapValueExpressionPriv 
{
    private:
    friend class MapValueExpression ;
    MapValueExpressionPriv(
        AbstractKeyValMapPtr &map,
        ExpressionPtr &key,
        ExpressionPtr &dflt
    ):map_(map),key_(key),dflt_(dflt)
    {
        char_cstr_=new CharExpression(no_init);
        cstr_=char_cstr_;
		key_list_ = false;
    };
    MapValueExpressionPriv(
        AbstractKeyValMapPtr &map,
        ExpressionList &list,
        ExpressionPtr &dflt
    ):map_(map),list_(list),dflt_(dflt)
    {
        char_cstr_=new CharExpression(no_init);
        cstr_=char_cstr_;
		key_list_ = true;
    };
	bool key_list_;
    AbstractKeyValMapPtr map_ ;
    ExpressionPtr key_ ;
    ExpressionList list_ ;
    ExpressionPtr dflt_ ;
    ExpressionPtr cstr_ ;
    ptr< CharExpression >  char_cstr_ ;
};

MapValueExpression::MapValueExpression(
        AbstractKeyValMapPtr &map,
        ExpressionList &list,
        ExpressionPtr &dflt
)
{
    d_=new MapValueExpressionPriv( map, list, dflt );
};

MapValueExpression::MapValueExpression(
        AbstractKeyValMapPtr &map,
        ExpressionPtr &key,
        ExpressionPtr &dflt
)
{
    d_=new MapValueExpressionPriv( map, key, dflt );
};

MapValueExpression::~MapValueExpression()
{
    delete d_;
}

void
MapValueExpression::notify() 
{
    //AbstractKeyValMap does notify() so it can avoid calling
    //default notify if a value is found.
	if(!d_->key_list_)
	{
    	d_->char_cstr_->set( d_->map_->get( d_->key_, d_->dflt_ ));
	}
	else
	{
    	d_->char_cstr_->set( d_->map_->get( d_->list_, d_->dflt_ ));
	}
}

ExpressionPtr &
MapValueExpression::getProxy() const
{
    return d_->cstr_;
}

