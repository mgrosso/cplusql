// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PreSortedProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:02:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PreSortedProjection.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef PRESORTEDPROJECTION_H
#define PRESORTEDPROJECTION_H 1

#include "ProxyRelation.h"
#include "AbstractProjection.h"
#include "RelationMeta.h"
#include "ptr_strdup.h"

class PreSortedProjectionPriv ;

class PreSortedProjection : public ProxyRelation
{
    public:
    PreSortedProjection(
        DelimitedSourcePtr  &src,
        RelationMetaPtr     &meta,
        ExpressionList      &projection,
        MetricList          &metrics
    );
    virtual             ~PreSortedProjection();
    virtual int         lineNum() ;

    protected:
    virtual bool        childNotify();
    virtual bool        childNotifyLast();

    private:
    void load();
    void shuffle();
    bool compare();
    void copy_projection_compare();

    PreSortedProjectionPriv *d_ ;

    //not implemented
    PreSortedProjection( const PreSortedProjection &rhs);
    PreSortedProjection &operator=( const PreSortedProjection &rhs);
};

#endif /* PRESORTEDPROJECTION_H */
