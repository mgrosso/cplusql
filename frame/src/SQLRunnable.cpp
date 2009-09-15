#ident "file_id $Id: SQLRunnable.cpp,v 1.1 2004/02/20 21:53:04 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SQLRunnable.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb 16 18:43:08 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: SQLRunnable.cpp,v 1.1 2004/02/20 21:53:04 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SQLRunnable.h"
#include "ptr_strdup.h"
#include "BaseException.h"

class SQLRunnablePriv
{
    private:
    friend class SQLRunnable ;

    DBConnectionPtr         c_ ;
    char_ptr                sql_ ;
    int                     rows_affected_ ;

    SQLRunnablePriv( DBConnectionPtr &c, const char *sql, int rows )
    :c_(c),sql_(ptr_strdup(sql)),rows_affected_(rows)
    {
        ;
    };
};

SQLRunnable::SQLRunnable(
                        DBConnectionPtr &conn, 
                        const char *sql, 
                        int rows_affected
                        )
{
    d_=new SQLRunnablePriv ( conn, sql, rows_affected );
}

SQLRunnable::~SQLRunnable()
{
    delete d_;
}

void
SQLRunnable::run()
{
    StatementPtr s= d_->c_->createStatement();
    int rows = s->executeUpdate( d_->sql_.get());
    if( d_->rows_affected_ != -1 && rows != d_->rows_affected_ )
    {
        PANICV( "SQLRunnable::run(): expected to change %i rows, affected %i",
                d_->rows_affected_, rows );
    }
}




