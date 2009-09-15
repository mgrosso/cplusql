// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SimpleRelation.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Nov 10 00:00:19 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SimpleRelation.h,v 1.4 2005/01/05 02:47:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SIMPLERELATION_H
#define SIMPLERELATION_H 1

#include "ptr.h"
#include "AbstractDelimitedSource.h"
#include "ProxyRelation.h"
#include "RelationMeta.h"
#include "Expression.h"

class SimpleRelationPriv ;

class SimpleRelation : public ProxyRelation
{
    public:
    SimpleRelation(
        DelimitedSourcePtr &src,
        RelationMetaPtr &meta,
        ExpressionList &derivedColumns,
        ExpressionList &filters,
        bool only_derived = false
    );
    virtual             ~SimpleRelation();
    virtual int         lineNum();
    
    protected:
    virtual bool        childNotify();
    virtual bool        childNotifyLast();

    private:
    SimpleRelationPriv *d_;

    //not implemented
    SimpleRelation( const SimpleRelation &rhs);
    SimpleRelation &operator=( const SimpleRelation &rhs);
};

#endif /* SIMPLERELATION_H */
