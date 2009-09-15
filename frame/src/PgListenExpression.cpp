#ident "file_id $Id: PgListenExpression.cpp,v 1.2 2005/10/19 08:41:19 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PgListenExpression.cpp
// Author:    mgrosso 
// Created:   Sun Jan  9 04:21:31 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: PgListenExpression.cpp,v 1.2 2005/10/19 08:41:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "frame_config.h"
#ifdef FRAME_USES_POSTGRES

#include "PgListenExpression.h"
#include "db/PostgresConnection.h"
#include "ptr_strdup.h"
#include "AppLog.h"
#include "BaseException.h"

class PgListenExpressionPriv 
{
    private:
    DBConnectionPtr         conn_;
    PGconn *                pgconn_ ;
    int                     fd_ ;
    char_ptr                event_;

    friend class PgListenExpression ;

    PgListenExpressionPriv(DBConnectionPtr &conn, const char *event )
    :conn_(conn), event_(ptr_strdup(event))
    {
        PostgresConnection*     pgwrap =
            dynamic_cast<PostgresConnection *>(conn.get());
        pgconn_= pgwrap->getPostgresConnection();
        char_ptr sql = ptr_strdup("listen ") + event_ ; 
        PGresult *result = PQexec( pgconn_, sql.get());
        ExecStatusType ret = PQresultStatus( result );
        PGPANICIF( pgconn_, PGRES_COMMAND_OK, ret, sql.get(), "problem listening.");
        PQclear( result );
        fd_ = PQsocket( pgconn_ );
    };
};

PgListenExpression::PgListenExpression(
    DBConnectionPtr &conn, const char *event )
{
    d_=new PgListenExpressionPriv ( conn, event );
}

PgListenExpression::~PgListenExpression()
{
    delete d_;
}

const char *
PgListenExpression::getRaw()
{
    return "1";
}

const char *
PgListenExpression::getRaw() const
{
    return "1";
}

void
PgListenExpression::notify()
{
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(d_->fd_, &mask);
    int eintr = MAX_CONSECUTIVE_EINTR ;
    int ret ;
    PGnotify *n  ;

    if( 0==PQconsumeInput( d_->pgconn_ ))
    {
        PGPANIC( d_->pgconn_, "PQconsumeInput()" );
    }
    n = PQnotifies( d_->pgconn_ );

    while( !n )
    {
        APPLOG_DBG("waiting in select() ... ");
        while(-1==(ret=select(d_->fd_+1, &mask,NULL, NULL, NULL )) && eintr )
        {
            APPLOG_WARN("eintr select()ing on fd %i", d_->fd_ );
        }
        if( ! eintr )
        {
            PANICV( "got EINTR  MAX_CONSECUTIVE_EINTR(%i) times while trying to select for read on postgres fd %i",
                MAX_CONSECUTIVE_EINTR, d_->fd_ );
        }
        if( 0==PQconsumeInput( d_->pgconn_ ))
        {
            PGPANIC( d_->pgconn_, "PQconsumeInput()" );
        }
        n  = PQnotifies( d_->pgconn_ );
    }
    APPLOG_DBG("notification received: %s, from pid = %i",
        n->relname, n->be_pid );
    PQfreemem( n );
}

#endif /* FRAME_USES_POSTGRES */
