// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestJoint.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 21:15:04 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DestJoint.h,v 1.5 2005/02/01 01:26:01 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DESTJOINT_H
#define DESTJOINT_H 1

#include "AbstractRelation.h"
#include "Dest.h"
#include "ptr_strdup.h"
#include "Listener.h"

class DestJointPriv;

class DestJoint : public Listener
{
    public:

    DestJoint( RelationPtr &rel, pDest &dest, const char *delimiter, 
            const char *record_separator, const char * metafile=NULL, bool first_row_header = false );

    virtual ~DestJoint();
    virtual void notify();

    private:
    DestJointPriv *jp_;
    

    //not implemented
    DestJoint();
    DestJoint( const DestJoint &rhs );
    DestJoint & operator=( const DestJoint &rhs );
};

#endif /* DESTJOINT_H */
