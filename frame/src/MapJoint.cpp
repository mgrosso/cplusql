#ident "file_id $Id: MapJoint.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MapJoint.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 21:24:09 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MapJoint.cpp,v 1.7 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "MapJoint.h"
#include "BaseException.h"

#include "dbg.h"

class MapJointPriv
{
    private:
    friend class MapJoint;
    RelationPtr rel_;
    AbstractKeyValSetMapPtr map_;
    ExpressionPtr       key_;
    ExpressionPtr       val_;
    MapJointPriv( 
        RelationPtr &rel,
        AbstractKeyValSetMapPtr &map,
        ExpressionPtr       &key,
        ExpressionPtr       &val
    ):rel_(rel),map_(map),key_(key),val_(val)
    {
        ;//noop
    };
};

MapJoint::MapJoint( 
    RelationPtr &rel, 
    AbstractKeyValSetMapPtr &emap, 
    ExpressionPtr &key, 
    ExpressionPtr &val 
)
:Listener()
{
    jp_=new MapJointPriv(rel,emap,key,val);
    rel->registerListener( this );
}

MapJoint::~MapJoint()
{
    delete jp_;
}

void
MapJoint::notify()
{
    jp_->key_->notify();
    jp_->val_->notify();
    jp_->map_->set( jp_->key_, jp_->val_ );
}

