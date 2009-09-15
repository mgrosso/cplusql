// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MapValueExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:08:07 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MapValueExpression.h,v 1.6 2004/01/22 18:28:07 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MAPVALUEEXPRESSION_H
#define MAPVALUEEXPRESSION_H 1

#include "ProxyExpression.h"
#include "AbstractKeyValMap.h"

class MapValueExpressionPriv ;

class MapValueExpression : public ProxyExpression 
{
    public:
    MapValueExpression( AbstractKeyValMapPtr &map, ExpressionPtr &key, ExpressionPtr &dflt );
    MapValueExpression( AbstractKeyValMapPtr &map, ExpressionList &list, ExpressionPtr &dflt );
    virtual                     ~MapValueExpression();
    virtual void                notify();
    virtual ExpressionPtr &     getProxy() const ;

    protected:

    private:
    MapValueExpressionPriv *d_;
    //not implemented
    MapValueExpression();
    MapValueExpression(const MapValueExpression &rhs);
    MapValueExpression &operator=(const MapValueExpression &rhs);
};

#endif /* MAPVALUEEXPRESSION_H */

