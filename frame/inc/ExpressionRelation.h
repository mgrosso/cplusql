
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExpressionRelation.h
// Author:    mgrosso 
// Created:   Sun Jan  9 00:25:09 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: ExpressionRelation.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef EXPRESSIONRELATION_H
#define EXPRESSIONRELATION_H 1

#include "Expression.h"
#include "RelationMeta.h"
#include "AbstractDelimitedSourceNext.h"
#include "ProxyRelation.h"

class ExpressionRelationPriv ;
class ExpressionRelation : 
    virtual public AbstractDelimitedSourceNext, 
    virtual public ProxyRelation
{
    public:
    ExpressionRelation(
        ExpressionList &columns,
        RelationMetaPtr &meta,
        ExpressionPtr &whiletrue
    );
    virtual ~ExpressionRelation();

    //next() notifies expressions, then returns true.
    virtual bool                next();
    virtual int                 lineNum();

    virtual bool                childNotify(); //noop.  not used.
    virtual void                notify();   //noop, not used.

    private:
    ExpressionRelationPriv *d_;
    //not implemented
    ExpressionRelation();
    ExpressionRelation( const ExpressionRelation &rhs );
    ExpressionRelation &operator=( const ExpressionRelation &rhs );
};

#endif /* EXPRESSIONRELATION_H */
