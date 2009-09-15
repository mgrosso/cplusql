// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RelationMeta.h
// Author:    mgrosso Matthew Grosso
// Created:   Tue Nov  5 05:06:59 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: RelationMeta.h,v 1.5 2005/01/24 18:47:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef RELATIONMETA_H
#define RELATIONMETA_H 1

#include "AbstractRelationMeta.h"

class RelationMetaPriv ;

class RelationMeta :virtual public AbstractRelationMeta 
{
private:
    RelationMetaPriv *rm_;

public:
    RelationMeta();
    RelationMeta(const RelationMeta &rhs );
    RelationMeta & operator=(const RelationMeta &rhs );
    virtual ~RelationMeta();
    void            setRelationName( const char *relname );
    void            addColumn( const char *colname );
    void            addColumns( list< char_ptr > &columns );

    virtual         const char *    getRelationName()const ;
    virtual         char_ptr        getRelationHeader()const ;
    virtual         size_t          getNumColumns()const ;
    virtual         const char *    getColumnName( size_t index )const ;
    virtual         size_t          getColumnIndex( const char *name )const ;
};

typedef ptr< RelationMeta > RelationMetaPtr ;

#endif /* RELATIONMETA_H */
