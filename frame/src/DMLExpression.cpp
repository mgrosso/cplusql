#ident "file_id $Id: DMLExpression.cpp,v 1.1 2005/01/24 19:40:29 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DMLExpression.cpp
// Author:    mgrosso 
// Created:   Fri Jan  7 14:47:58 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: DMLExpression.cpp,v 1.1 2005/01/24 19:40:29 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "DMLExpression.h"
#include "BaseException.h"
#include "AppLog.h"

class DMLExpressionPriv 
{
    const char *                one_;
    const char *                zero_;
    const char *                raw_;
    DBConnectionPtr             db_ ;
    StatementPtr                statement_ ;
    ExpressionList              begin_ ;
    ExpressionList              sql_ ;
    ExpressionList              commit_ ;
    size_t                      rows_per_transaction_ ;
    size_t                      rows_so_far_;

    private:
    friend class DMLExpression ;
    DMLExpressionPriv( 
        DBConnectionPtr &db,
        ExpressionList &beginsql,
        ExpressionList &sql, 
        ExpressionList &commitsql,
        size_t rows_per_transaction
    )
    :one_("1"),zero_("0"),raw_(zero_),
    db_(db), statement_(db_->createStatement()),
    begin_(beginsql), sql_(sql), commit_(commitsql),
    rows_per_transaction_(rows_per_transaction), rows_so_far_(0)
    {
        ;//noop
    };
    void notify();
    void runList( ExpressionList &statements );
};

DMLExpression::DMLExpression(
        DBConnectionPtr &db,
        ExpressionList &beginsql,
        ExpressionList &sql, 
        ExpressionList &commitsql,
        size_t rows_per_transaction
)
{
    d_=new DMLExpressionPriv (
        db, beginsql, sql, commitsql, rows_per_transaction
    );
}

DMLExpression::~DMLExpression()
{
    delete d_;
}

const char *
DMLExpression::getRaw() 
{
    return d_->raw_;
}

const char *
DMLExpression::getRaw() const
{
    return d_->raw_;
}

void
DMLExpression::notify()
{
    d_->notify();
}

void
DMLExpression::notifyLast()
{
    if(d_->rows_so_far_ != 0 )
    {
        d_->runList( d_->commit_ );
        d_->rows_so_far_ = 0 ;
    }
}

void
DMLExpressionPriv::notify()
{
    if( 0 == rows_so_far_ )
    {
        runList( begin_ );
    }
    runList( sql_ );
    if( ++rows_so_far_ == rows_per_transaction_ )
    {
        runList( commit_ );
        rows_so_far_ = 0 ;
    }
}

void
DMLExpressionPriv::runList( ExpressionList &s )
{
    ExpressionListItr i, e;
    for( i=s->begin(), e=s->end(); i!=e; ++i )
    {
        ExpressionPtr &ep = (*i);
        ep->notify();
        const char *s=ep->getRaw();
        if(!s || !*s )
        {
            //APPLOG_DBG("skip empty query");
            continue;
        }
        APPLOG_DBG("query: %s", s);
        int rows = statement_->executeUpdate( s);
        APPLOG_DBG("dml query %i/%i results: %i", rows_so_far_, rows_per_transaction_, rows );
    }
}

