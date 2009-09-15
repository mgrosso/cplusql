#ident "file_id $Id: PgCopyByteSource.cpp,v 1.3 2005/10/19 08:41:19 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PgCopyByteSource.cpp
// Author:    mgrosso 
// Created:   Wed Nov 17 22:06:06 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: PgCopyByteSource.cpp,v 1.3 2005/10/19 08:41:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "frame_config.h"
#ifdef FRAME_USES_POSTGRES

#include "libpq-fe.h"
#include <string>
#include <strings.h>

#include "PgCopyByteSource.h"
#include "BaseException.h"
#include "db/PostgresConnection.h"
#include "AppLog.h"

using namespace std;

class PgCopyByteSourcePriv 
{
    private:
    DBConnectionPtr         dbconn_ ;
    PGconn *                pgconn_ ;
    PostgresConnection*     pgwrap_ ;
    string                  sql_ ;
    char *                  buf_ ;
    char *                  cursor_ ;
    size_t                  bufsz_ ;

    friend class PgCopyByteSource ;
    PgCopyByteSourcePriv()
    :buf_(NULL),cursor_(NULL),bufsz_(0)
    {
        ;
    };
};

PgCopyByteSource::PgCopyByteSource( 
    DBConnectionPtr conn, const char *table_name )
{
    d_=new PgCopyByteSourcePriv ();
    d_->dbconn_ = conn;
    d_->pgwrap_ = dynamic_cast<PostgresConnection *>(conn.get());
    if(!d_->pgwrap_ )
    {
        PANICV("cant get PostgresConnection from DBConnection");
    }
    d_->pgconn_= d_->pgwrap_->getPostgresConnection();
    if( ! d_->pgconn_ )
    {
        PANICV("cant get PGconn * from PostgresConnection");
    }
    d_->sql_ = "COPY ";
    d_->sql_ = d_->sql_ + table_name;
    d_->sql_ = d_->sql_ + " TO STDIN WITH DELIMITER '|' NULL '' ";
    PGresult *result = PQexec( d_->pgconn_, d_->sql_.c_str());
    ExecStatusType ret = PQresultStatus( result );
    if( ret != PGRES_COPY_OUT )
    {
        //note: need to factor this out, as it is duplicated in next()
        int fd = PQsocket( d_->pgconn_ );
        int backend = PQbackendPID( d_->pgconn_ );
        ConnStatusType status = PQstatus ( d_->pgconn_ );
        int version = PQprotocolVersion ( d_->pgconn_ );
        char *pgerr = PQerrorMessage( d_->pgconn_ );
        char *reserr = PQresultErrorMessage( result );
        PANICV( "PQexec did not return PGRES_COPY_OUT[%i],instead it return %i. connection errmsg[%s] result errmsg[%s]: fd=%i backend=%i status=%s version=%i sql=[%s]",
                    PGRES_COPY_OUT, ret, 
                    pgerr, reserr,
                    fd, backend, 
                    status == CONNECTION_OK ? "OK" : "BAD" ,
                    version, d_->sql_.c_str());
    }
}

PgCopyByteSource::~PgCopyByteSource()
{
    delete d_;
}

bool
PgCopyByteSource::next( char *buffer, size_t &bytes_read, size_t bufsize )
{
    //APPLOG_DBG(".");
    if( d_->buf_ )
    {
        strncpy( buffer, d_->cursor_, bufsize );
        if( bufsize > d_->bufsz_ )
        {
            PQfreemem( d_->buf_);
            d_->bufsz_ = 0;
            d_->buf_=NULL;
            d_->cursor_=NULL;
        } else
        {
            d_->bufsz_ -= bufsize;
            d_->cursor_ += bufsize;
        }
        //APPLOG_DBG(".");
        return true;
    }
    //APPLOG_DBG(".");
    char ** mybuffer = & d_->buf_ ;
    int ret = PQgetCopyData( d_->pgconn_, mybuffer, 0 );
    switch( ret ) {
        case    -2 :
            {
                int fd = PQsocket( d_->pgconn_ );
                int backend = PQbackendPID( d_->pgconn_ );
                ConnStatusType status = PQstatus ( d_->pgconn_ );
                int version = PQprotocolVersion ( d_->pgconn_ );
                char *pgerr = PQerrorMessage( d_->pgconn_ );
            
                PANICV( "PQgetCopyData returned error. fd=%i backend=%i status=%s version=%i errmsg=[%s]  sql=[%s]",
                    fd, backend, 
                    status == CONNECTION_OK ? "OK" : "BAD" ,
                    version, pgerr, d_->sql_.c_str());
            }
            break;
        case -1 :
            //no more data;
            return false;
            break;
        case 0:
            PANICV( "PQgetCopyData returned zero, which means no data ready yet, but we dont support asynchonous COPY yet!");
            break;
        default:
            if( ret > static_cast<int>(bufsize ))
            {
                //APPLOG_DBG(".");
                strncpy( buffer, *mybuffer, bufsize );
                *(buffer+bufsize-1) = '\0';
                d_->cursor_=*mybuffer+bufsize ;
                d_->bufsz_ = ret - bufsize ;
                bytes_read = bufsize ;
            }
            else
            {
                //APPLOG_DBG(".");
                strcpy( buffer, *mybuffer );
                PQfreemem( *mybuffer );
                *mybuffer = NULL;
                *(buffer+ret)='\0';
                bytes_read = ret;
            }
            //APPLOG_DBG( "returning %lu bytes: %s", bytes_read, buffer );
    }
    return true;
}

#endif /* FRAME_USES_POSTGRES */
