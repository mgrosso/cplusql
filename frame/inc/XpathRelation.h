
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      XpathRelation.h
// Author:    mgrosso 
// Created:   Thu Jan  6 02:10:38 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: XpathRelation.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef XPATHRELATION_H
#define XPATHRELATION_H 1


#include "ProxyRelation.h"


class XpathRelationPriv ;
class XpathRelation : public ProxyRelation
{
    public:
    virtual int                 lineNum();
    virtual bool                childNotify();
    virtual bool                childNotifyLast();
    virtual void                notify();
    virtual                     ~XpathRelation();
    XpathRelation( 
        ExpressionPtr &xmltext, RelationMetaPtr &meta, const char *query);

    private:
    XpathRelationPriv *d_;
    //not implemented
    XpathRelation();
    XpathRelation( const XpathRelation &rhs );
    XpathRelation &operator=( const XpathRelation &rhs );
};

#endif /* XPATHRELATION_H */
