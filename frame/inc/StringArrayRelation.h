
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArrayRelation.h
// Author:    mgrosso 
// Created:   Thu Jul  8 01:44:38 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArrayRelation.h,v 1.2 2004/09/21 16:55:51 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRINGARRAYRELATION_H
#define STRINGARRAYRELATION_H 1

#include "ProxyRelation.h"
#include "StringArray.h"
#include "Runnable.h"
#include "RelationMeta.h"

class StringArrayRelationPriv ;
class StringArrayRelation : public ProxyRelation, public Runnable
{
    public:
    StringArrayRelation( RelationMetaPtr &meta, StringArrayPtr &sa);
    virtual                             ~StringArrayRelation();
    virtual int                         lineNum();
    virtual bool                        childNotify();
    virtual void                        run();

    private:
    StringArrayRelationPriv *d_;
    //not implemented
    StringArrayRelation();
    StringArrayRelation( const StringArrayRelation &rhs );
    StringArrayRelation &operator=( const StringArrayRelation &rhs );
};

typedef ptr< StringArrayRelation > StringArrayRelationPtr ;

#endif /* STRINGARRAYRELATION_H */
