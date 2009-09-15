
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArrayOrderedRelation.h
// Author:    mgrosso 
// Created:   Sun Aug 15 21:20:29 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArrayOrderedRelation.h,v 1.1 2004/09/21 16:57:51 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRINGARRAYORDEREDRELATION_H
#define STRINGARRAYORDEREDRELATION_H 1

#include "ProxyRelation.h"
#include "Runnable.h"
#include "StringArray.h"
#include "RelationMeta.h"
#include "Index.h"

class StringArrayOrderedRelationPriv ;
class StringArrayOrderedRelation : public ProxyRelation, public Runnable
{
    public:
    StringArrayOrderedRelation( RelationMetaPtr &rm, StringArrayPtr &sa,
        Index::Iterator bottom_inclusive, Index::Iterator top_exclusive  );

    StringArrayOrderedRelation( RelationMetaPtr &rm, StringArrayPtr &sa,
        IndexPtr &whole_index );

    virtual                             ~StringArrayOrderedRelation();
    void                                reset(
        Index::Iterator bottom_inclusive, Index::Iterator top_exclusive  );
    virtual int                         lineNum();
    virtual bool                        childNotify();
    virtual void                        run();
    private:
    StringArrayOrderedRelationPriv *d_;
    void                                initv();
    //not implemented
    StringArrayOrderedRelation();
    StringArrayOrderedRelation( const StringArrayOrderedRelation &rhs );
    StringArrayOrderedRelation &operator=( const StringArrayOrderedRelation &rhs );
};



#endif /* STRINGARRAYORDEREDRELATION_H */
