// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MergeJoinRelation.h
// Author:    mgrosso Matthew Grosso
// Created:   Thu Dec  5 00:25:54 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MergeJoinRelation.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef MERGEJOINRELATION_H
#define MERGEJOINRELATION_H 1
#include "ProxyRelation.h"

class MergeJoinRelationPriv;

class MergeJoinRelation : public ProxyRelation
{
    public:
    //convenience method, if you want to use default column naming
    //strategy.
    static MergeJoinRelation * makeMergeJoinRelation(
        RelationPtr left, RelationPtr right, const char *name,
        ExpressionPtr leftexp, ExpressionPtr rightexp,
        bool str
    );

    MergeJoinRelation(  
        RelationPtr left, RelationPtr right, RelationMetaPtr meta,
        ExpressionPtr leftexp, ExpressionPtr rightexp,
        bool str
        );

    virtual             ~MergeJoinRelation();
    virtual int         lineNum() ;
    
    protected:
    virtual bool        childNotify();

    private:
    MergeJoinRelationPriv    *   d_;

    //not implemented
    MergeJoinRelation( const MergeJoinRelation &rhs);
    MergeJoinRelation &operator=( const MergeJoinRelation &rhs);
};

#endif /* MERGEJOINRELATION_H */
