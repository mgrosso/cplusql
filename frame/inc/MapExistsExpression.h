// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MapExistsExpression.h
// Author:    mgrosso 
// Created:   Mon Mar 31 01:15:16 EST 2003 on caliban.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: MapExistsExpression.h,v 1.6 2004/01/22 18:28:07 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef MAPEXISTSEXPRESSION_H
#define MAPEXISTSEXPRESSION_H 1
#include "BooleanExpression.h"
#include "AbstractKeyValMap.h"

class MapExistsExpressionPriv ;

class MapExistsExpression : public BooleanExpression 
{
    public:
    MapExistsExpression(
        AbstractKeyValMapPtr &map,
        ExpressionPtr &key
    );
    MapExistsExpression(
        AbstractKeyValMapPtr &map,
        ExpressionList &key
    );
    virtual ~MapExistsExpression();
    virtual void                notify() ;
    virtual bool                isNull() const ;
    virtual bool                getBool() const ;

    protected:

    private:
    MapExistsExpressionPriv *d_;
    //not implemented
    MapExistsExpression();
    MapExistsExpression(const MapExistsExpression &rhs);
    MapExistsExpression &operator=(const MapExistsExpression &rhs);
};

#endif /* MAPEXISTSEXPRESSION_H */

