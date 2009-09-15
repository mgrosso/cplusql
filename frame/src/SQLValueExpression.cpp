#ident "file_id $Id: SQLValueExpression.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SQLValueExpression.cpp
// Author:    mgrosso 
// Created:   Sun Jan  9 22:01:15 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SQLValueExpression.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SQLValueExpression.h"
#include "BaseException.h"
#include "AppLog.h"

class SQLValueExpressionPriv 
{
    private:
    DBConnectionPtr             db_ ;
    StatementPtr                statement_ ;
    ResultSetPtr                rs_ ;
    ExpressionPtr               sql_ ;
    bool                        panicifnotfound_ ;
    const char *                rawval_;

    friend class SQLValueExpression ;

    SQLValueExpressionPriv(
        DBConnectionPtr &db, 
        ExpressionPtr &sql, 
        bool panicifnotfound
    ):  db_(db), 
        statement_( db_->createStatement()),
        sql_(sql), 
        panicifnotfound_(panicifnotfound)
    {
        //noop
    };
};

SQLValueExpression::SQLValueExpression(
    DBConnectionPtr &db, 
    ExpressionPtr &sql, 
    bool panicifnotfound 
)
{
    d_=new SQLValueExpressionPriv (
        db, sql, panicifnotfound);
}

SQLValueExpression::~SQLValueExpression()
{
    delete d_;
}

const char *
SQLValueExpression::getRaw()
{
    return d_->rawval_;
}

const char *
SQLValueExpression::getRaw() const
{
    return d_->rawval_;
}

void
SQLValueExpression::notify()
{
    d_->rs_ = NULL;
    d_->sql_->notify();
    const char *q = d_->sql_->getRaw();
    APPLOG_DBG("about to run [ %s ]", q );
    d_->rs_ = d_->statement_->executeQuery(q);
    if( d_->rs_->next())
    {
        d_->rawval_ = d_->rs_->getString(0);
    }
    else if( d_->panicifnotfound_ )
    {
        PANICV("no rows found. q=[%s]", q );
    }
    else
    {
        d_->rawval_ = "";
    }
    APPLOG_DBG("done. value=%s", d_->rawval_ );
}

