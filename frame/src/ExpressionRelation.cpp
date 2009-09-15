#ident "file_id $Id: ExpressionRelation.cpp,v 1.1 2005/01/24 19:40:31 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExpressionRelation.cpp
// Author:    mgrosso 
// Created:   Sun Jan  9 00:25:09 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: ExpressionRelation.cpp,v 1.1 2005/01/24 19:40:31 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ExpressionRelation.h"

class ExpressionRelationPriv 
{
    private:
    friend class ExpressionRelation ;

    ExpressionList              columns_ ;
    RelationMetaPtr             meta_ ;
    ExpressionPtr               whiletrue_ ;
    int                         line_;

    ExpressionRelationPriv(
        ExpressionList &columns,
        RelationMetaPtr &meta,
        ExpressionPtr &whiletrue
    ): columns_( columns ), meta_( meta ), whiletrue_(whiletrue),line_(0)
    {
        //TODO
    };
};

ExpressionRelation::ExpressionRelation(
        ExpressionList &columns,
        RelationMetaPtr &meta,
        ExpressionPtr &whiletrue
        )
:AbstractDelimitedSourceNext(),ProxyRelation(meta,columns )
{
    d_=new ExpressionRelationPriv ( columns, meta, whiletrue );
}

ExpressionRelation::~ExpressionRelation()
{
    delete d_;
}

bool
ExpressionRelation::next()
{
    d_->whiletrue_->notify();
    if( d_->whiletrue_->getBool())
    {
        ExpressionListItr i,e;
        for( i=d_->columns_->begin(), e=d_->columns_->end() ; i!=e; ++i)
        {
            (*i)->notify();
        }
        ++d_->line_;
        return true;
    }
    return false;
}

int
ExpressionRelation::lineNum()
{
    return d_->line_;
}

bool
ExpressionRelation::childNotify()
{
    //not used.
    return true;
}

void
ExpressionRelation::notify()
{
    n_->notify();
}

