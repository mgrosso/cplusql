#ident "file_id $Id: DestPgCopy.cpp,v 1.5 2005/03/11 19:46:14 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestPgCopy.cpp
// Author:    mgrosso 
// Created:   Sat May 29 16:03:44 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: DestPgCopy.cpp,v 1.5 2005/03/11 19:46:14 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "frame_config.h"
#ifdef FRAME_USES_POSTGRES

#include <list>
#include <string>
#include <string.h>

#include "DestPgCopy.h"
#include "ptr_strdup.h"
#include "db/PostgresConnection.h"
#include "BaseException.h"
#include "AppLog.h"

class DestPgCopyPriv 
{
    private:
    friend class DestPgCopy ;

    // we keep convenience pointers to pg stuff, but must also 
    // keep a reference to the parent ptr<> so the pg stuff is not
    // closed before we're done with it.
    PGconn *            pgconn_ ;
    PostgresConnection* pgwrap_ ;
    DBConnectionPtr     dbconn_ ;

    list< char * >      backlog_ ;
    list< int >         backlog_sizes_ ;
    int                 cached_size_;
    int                 max_cached_size_;

    bool                trySend( const char *s, int len);                 
    bool                trySendFront();                 
    bool                cache( const char *s, int len);                 
    void                popcache( int sz);                 
    void                waitToWrite();
};


bool
DestPgCopyPriv::cache(const char *s, int sz )
{
    if( sz + cached_size_ > max_cached_size_)
    {
        return false;
    }
    backlog_.push_back( strdup_wnew( s ));
    backlog_sizes_.push_back( sz );
    cached_size_ += sz ;
    return true ;
}

void
DestPgCopyPriv::popcache( int sz )
{
    backlog_.pop_front();
    backlog_sizes_.pop_front();
    cached_size_ -= sz ;
}

bool
DestPgCopyPriv::trySend(const char *s, int sz )
{
    int result = PQputCopyData( pgconn_, s, sz );
    if( result == -1 )
    {
        PANICV("Could not PQputCopyData(); %s", PQerrorMessage(pgconn_));
    }
    //APPLOG_DBG("sent %i bytes(%s)", sz, s );
    return result;
}

bool
DestPgCopyPriv::trySendFront( void )
{
    int sz = backlog_sizes_.front();
    bool result = trySend( backlog_.front(), sz );
    if( result )
    {
        popcache( sz );
    }
    return result;
}

void DestPgCopyPriv::waitToWrite()
{
    fd_set mask;
    int fd = PQsocket( pgconn_ );
    if( fd < 0 )
    {
        PANICV("bad fd back from PQsocket(); %s", PQerrorMessage(pgconn_));
    }
    FD_ZERO(&mask);
    FD_SET(fd, &mask);
    //struct timeval tv;
    //tv.tv_sec = 0;
    //tv.tv_usec = 100 ;
    if( -1 == select( fd + 1, NULL, &mask, NULL, NULL ))
    {
        PANICV("could not select on fd %i for table ", fd );
    }
}

DestPgCopy::DestPgCopy( 
    DBConnectionPtr &conn, int maxcache, const char *tablename, bool blank_is_null )
{
    d_=new DestPgCopyPriv ();

    d_->dbconn_ = conn ;
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

    d_->max_cached_size_ = maxcache ;
    d_->cached_size_ = 0;

    string sql = "COPY ";
    sql = sql + tablename;
    sql = sql + " FROM STDIN WITH DELIMITER '|' ";
    if( blank_is_null )
    {
        sql = sql + "NULL '' ";
    }else
    {
        sql = sql + "NULL '__NULL__' ";
    }
    //fprintf(stderr, "exec sql: %s\n", sql.c_str());
    APPLOG_DBG( "running COPY sql: %s", sql.c_str());
    (void)d_->pgwrap_->execute( sql.c_str(), PGRES_COPY_IN );
    APPLOG_DBG( "ran COPY sql ok, ready to start sending rows" );
}

DestPgCopy::~DestPgCopy()
{
    delete d_;
}

void
DestPgCopy::put( const char *s )
{
    put( s, strlen(s));
}

void
DestPgCopy::put( const char *s, int slen )
{
    //fprintf(stderr, "DestPgCopy::put(): len=%i s=%s\n", slen, s );
    //send as much of the backlog as we can...
    while( d_->backlog_.size() && d_->trySendFront())
    {
        ;//noop
    }
    //if there is backlog remaining, try to cache this write.
    if( d_->backlog_.size())
    {
        if( d_->cache( s, slen ))
        {
            //we did as much as we could without blocking for now.
            return;
        }
        //have a backlog, and cannnot cache
        flush();
    }
    //at this point we have no backlog, just the one write.
    while(! d_->trySend( s, slen ))
    {
        if( d_->cache( s, slen ))
        {
            return;
        }
        d_->waitToWrite();
    }
}

void
DestPgCopy::flush( void )
{
    while( d_->backlog_.size())
    {
        d_->waitToWrite();
        (void)d_->trySendFront();
    }
}

void
DestPgCopy::finish( void )
{
    int ret = 0;
    APPLOG_DBG( "flushing..." );
    flush();
    APPLOG_DBG( "ending.. backlog size=%lu backlog_sizes=%lu cached_size=%i max_cached_size_=%i",
        d_->backlog_.size() , d_->backlog_sizes_.size() , 
        d_->cached_size_, d_->max_cached_size_
    );
    while( ret == 0 )
    {
        d_->waitToWrite();
        ret = PQputCopyEnd( d_->pgconn_, 0 );
        if( -1 == ret )
        {
            PANICV("Bad PQputCopyEnd() %s", PQerrorMessage(d_->pgconn_));
        }
    }
    (void)d_->pgwrap_->getResult( PGRES_COMMAND_OK );
    APPLOG_DBG( "done" );
}

#endif /* FRAME_USES_POSTGRES */
