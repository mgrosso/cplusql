#ident "file_id $Id: PgCopyListener.cpp,v 1.2 2005/03/11 19:52:40 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PgCopyListener.cpp
// Author:    mgrosso 
// Created:   Sat Nov 27 19:02:56 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: PgCopyListener.cpp,v 1.2 2005/03/11 19:52:40 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "PgCopyListener.h"
#include "Dest.h"
#include "SQLRunnable.h"
#include "DestPgCopy.h"
#include "AppLog.h"

class PgCopyListenerPriv 
{
    private:
    friend class PgCopyListener ;

    RelationPtr             source_;
    DBConnectionPtr         dbconn_;
    char_ptr                table_ ;
    char_ptr                delim_ ;
    char_ptr                postload_sql_ ;

    size_t                  columns_;
    pDest                   pgdest_ ;
    pDest                   bufdest_ ;
    size_t                  rows_;
    size_t                  bytes_ ;

    PgCopyListenerPriv(
        RelationPtr &source,
        DBConnectionPtr &dbconn,
        ConfigPtr &buffer_cfg,
        const char *table_name,
        const char *delim,
        const char *preload_sql,
        const char *postload_sql
    ):source_(source),
        dbconn_(dbconn),
        table_(ptr_strdup(table_name)),
        delim_(ptr_strdup(delim ? delim : "|" )),
        postload_sql_(ptr_strdup_wnull(postload_sql)),
        columns_( source->getNumColumns()),
        pgdest_( NULL ),
        rows_(0)
    {
        if( preload_sql && *preload_sql )
        {
            APPLOG_DBG("running preload_sql: %s", preload_sql );
            SQLRunnable s(dbconn, preload_sql);
            s.run();
            APPLOG_DBG("ran preload_sql ok" );
        }
        bool blank_is_null= buffer_cfg->getBool( "BLANK_IS_NULL", "1" );
        pgdest_= new DestPgCopy( dbconn, 0, table_name, blank_is_null );
        //bufdest_ = DestThreadedBuffer::factory( d_->pgdest_, buffer_cfg );
        bufdest_ = pgdest_ ;
    };
};

PgCopyListener::PgCopyListener(
        RelationPtr &source,
        DBConnectionPtr &dbconn,
        ConfigPtr &buffer_cfg,
        const char *table_name,
        const char *delim,
        const char *preload_sql,
        const char *postload_sql
){
    d_=new PgCopyListenerPriv (
        source,
        dbconn,
        buffer_cfg,
        table_name,
        delim,
        preload_sql,
        postload_sql
        );
    source->registerListener( this );
}

PgCopyListener::~PgCopyListener()
{
    delete d_;
}

void 
PgCopyListener::notify()
{
    static int dbg =0;
    ++dbg ;
    size_t i=0 ;
    d_->bufdest_->put(d_->source_->getRaw(i));
    //fprintf(stderr, "notify():%i %i %s\n", dbg, i, d_->source_->getRaw(i));
    for( ++i ; i< d_->columns_ ; ++i )
    {
        d_->bufdest_->put(d_->delim_.get());
        d_->bufdest_->put(d_->source_->getRaw(i));
        //fprintf(stderr, "notify():%i %i %s\n", dbg, i, d_->source_->getRaw(i));
    }
    d_->bufdest_->put("\n");
    if( ++d_->rows_ % 10000 == 0 )
    {
        APPLOG_DBG( "%i rows loaded ... ", d_->rows_ );
    }
}

void 
PgCopyListener::notifyLast()
{
    d_->bufdest_ = NULL;//forces flush through buffer and destpgcopy
    (dynamic_cast<DestPgCopy *>(d_->pgdest_.get()))->finish();
    APPLOG_DBG( "total of %i rows loaded ... ", d_->rows_ );
    const char *sql = d_->postload_sql_.get() ;
    if( sql && *sql )
    {
        APPLOG_DBG("running postload_sql: %s", sql );
        SQLRunnable s(d_->dbconn_, sql);
        s.run();
        APPLOG_DBG("ran postload_sql ok" );
    }
}

