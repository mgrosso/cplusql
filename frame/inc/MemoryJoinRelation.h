
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MemoryJoinRelation.h
// Author:    mgrosso 
// Created:   Sun Aug 22 19:05:04 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: MemoryJoinRelation.h,v 1.1 2004/09/21 16:19:27 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef MEMORYJOINRELATION_H
#define MEMORYJOINRELATION_H 1

#include "ProxyRelation.h"
#include "StringArray.h"
#include "Expression.h"
#include "Index.h"

class MemoryJoinRelationPriv ;
class MemoryJoinRelation : public ProxyRelation
{
    public:
    MemoryJoinRelation(
        RelationMetaPtr &meta,
        RelationPtr &left,
        StringArrayPtr &right,
        ExpressionPtr &left_key,
        IndexPtr &right_key_lookup,
        bool outer
    );
    virtual                             ~MemoryJoinRelation();
    virtual int                         lineNum();
    virtual void                        notify();
    virtual bool                        childNotify();
    static  MemoryJoinRelation *        make(
        RelationPtr &left,
        StringArrayPtr &right,
        RelationMetaPtr &right_meta,
        ExpressionPtr &left_key,
        IndexPtr &right_key_lookup,
        bool outer,
        const char *my_name
        );

    private:
    MemoryJoinRelationPriv *d_;
    //not implemented
    MemoryJoinRelation();
    MemoryJoinRelation( const MemoryJoinRelation &rhs );
    MemoryJoinRelation &operator=( const MemoryJoinRelation &rhs );
};



#endif /* MEMORYJOINRELATION_H */
