// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MapJoint.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 21:15:04 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MapJoint.h,v 1.4 2004/01/16 21:12:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MAPJOINT_H
#define MAPJOINT_H 1

#include "AbstractRelation.h"
#include "AbstractKeyValSetMap.h"
#include "ptr_strdup.h"
#include "Listener.h"

class MapJointPriv;

class MapJoint : public Listener
{
    public:

    MapJoint( RelationPtr &rel, AbstractKeyValSetMapPtr &esmp, 
            ExpressionPtr &key, ExpressionPtr &val );
    virtual ~MapJoint();
    virtual void notify();

    private:
    MapJointPriv *jp_;

    //not implemented
    MapJoint();
    MapJoint( const MapJoint &rhs );
    MapJoint & operator=( const MapJoint &rhs );
};

#endif /* MAPJOINT_H */
