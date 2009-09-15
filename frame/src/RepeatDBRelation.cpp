#ident "file_id $Id: RepeatDBRelation.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RepeatDBRelation.cpp
// Author:    mgrosso 
// Created:   Sun Jan  9 14:56:39 EST 2005 on upto11
// Project:   
// Purpose:   
//
// Copyright (c) 2005 
//
// $Id: RepeatDBRelation.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RepeatDBRelation.h"
#include "db/DBDelimitedSourcePriv.h"
#include "AppLog.h"

class RepeatDBRelationPriv 
{
    private:
    friend class RepeatDBRelation ;
    RepeatDBRelationPriv(
        DBConnectionPtr & db, ExpressionPtr &query
    ):db_(db), query_(query)
    {
        //TODO
    };
    DBConnectionPtr             db_;
    ExpressionPtr               query_ ;
};

RepeatDBRelation::RepeatDBRelation(
    DBConnectionPtr &db, ExpressionPtr &query, RelationMetaPtr &meta )
:DBDelimitedSource(), AbstractRelation(), RelationMeta(*meta)
{
    rd_=new RepeatDBRelationPriv (db, query);
    d_->stmt_ = rd_->db_->createStatement();
}

RepeatDBRelation::~RepeatDBRelation()
{
    delete rd_;
}

void
RepeatDBRelation::notify()
{
    rd_->query_->notify();
    const char *q = rd_->query_->getRaw();
    APPLOG_DBG("new q=%s", q );
    d_->result_ = NULL;//just to be paranoid, ensure old dtor goes first.
    d_->result_ = d_->stmt_->executeQuery(q);
    if( RelationMeta::getNumColumns() != DBDelimitedSource::getNumColumns())
    {
        PANICV(
            "mismatched # of columns, columnlist: %i and query: %i ", 
            RelationMeta::getNumColumns(), 
            DBDelimitedSource::getNumColumns()
        );
    }
    while( next())
    {
        n_->notify();
    }
}

bool
RepeatDBRelation::childNotify()
{
    //not used.
    return true;
}

char_ptr
RepeatDBRelation::getCharPtr()
{
    size_t colcnt = getNumColumns();
    charvec cl ;
    for( size_t i=0; i<colcnt; ++i )
    {
        cl.push_back(getRaw(i));
    }
    return ptr_strdup_list( cl, "|", false );
}

size_t
RepeatDBRelation::getNumColumns()const 
{
    return RelationMeta::getNumColumns();
}

