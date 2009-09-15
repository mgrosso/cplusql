// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProjectionRelation.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:02:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ProjectionRelation.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef PROJECTIONRELATION_H
#define PROJECTIONRELATION_H 1

#include "AbstractRelation.h"
#include "AbstractProjection.h"
#include "RelationMeta.h"

class ProjectionRelationPriv ;

class ProjectionRelation : public AbstractRelation
{
    public:
    ProjectionRelation(
        RelationPtr         src,
        RelationMetaPtr     meta,
        ExpressionList      projection,
        MetricList          metrics
    );
    virtual                 ~ProjectionRelation();
    virtual int             lineNum() ;
    virtual char_ptr        getCharPtr() ;

    //methods from AbstractDelimitedSource, left pure virtual
    virtual const char *    getRaw( size_t index );
    virtual char_ptr        getCharPtr( size_t index )const  ;
    virtual bool            getBool( size_t index )const  ;
    virtual long int        getLong( size_t index )const  ;
    virtual double          getDouble( size_t index )const  ;
    virtual long long int   getLongLong( size_t index )const  ;
    virtual long long int   getLongDouble( size_t index )const  ;

    //extensions
    virtual const char *    getRelationName()const ;
    virtual char_ptr        getRelationHeader()const ;
    virtual size_t          getNumColumns()const ;
    virtual const char *    getColumnName( size_t index )const ;
    virtual size_t          getColumnIndex( const char *col_name )const ;
    
    protected:
    virtual bool            childNotify();
    virtual bool            childNotifyLast();

    private:
    ExpressionPtr &at( int ) const ;
    ProjectionRelationPriv *d_ ;

    //not implemented
    ProjectionRelation( const ProjectionRelation &rhs);
    ProjectionRelation &operator=( const ProjectionRelation &rhs);
};

#endif /* PROJECTIONRELATION_H */
