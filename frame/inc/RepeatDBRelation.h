// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RepeatDBRelation.h
// Author:    mgrosso 
// Created:   Sun Jan  9 14:56:39 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: RepeatDBRelation.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef REPEATDBRELATION_H
#define REPEATDBRELATION_H 1

#include "db/DBDelimitedSource.h"
#include "AbstractRelation.h"
#include "RelationMeta.h"
#include "Expression.h"

class RepeatDBRelationPriv ;
class RepeatDBRelation :
    virtual public DBDelimitedSource,
    virtual public AbstractRelation,
    virtual public RelationMeta
{
    public:
    RepeatDBRelation( DBConnectionPtr & connection, ExpressionPtr &query,
        RelationMetaPtr &meta );
    virtual ~RepeatDBRelation();
    virtual void                notify();
    virtual bool                childNotify(); 
    virtual char_ptr            getCharPtr() ;

    virtual size_t              getNumColumns()const;

    private:
    RepeatDBRelationPriv *rd_;
    //not implemented
    RepeatDBRelation();
    RepeatDBRelation( const RepeatDBRelation &rhs );
    RepeatDBRelation &operator=( const RepeatDBRelation &rhs );
};



#endif /* REPEATDBRELATION_H */
