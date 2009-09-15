#ident "file_id $Id: PostgresMakeTable.cpp,v 1.5 2005/03/11 19:54:03 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresMakeTable.cpp
// Author:    mgrosso 
// Created:   Thu May 27 23:28:13 EDT 2004 on caliban
// Project:   
// Purpose:   
//
// $Id: PostgresMakeTable.cpp,v 1.5 2005/03/11 19:54:03 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "frame_config.h"
#ifdef FRAME_USES_POSTGRES

#include <string>

#include "PostgresMakeTable.h"
#include "SQLRunnable.h"
#include "BaseException.h"
#include "Dest.h"
#include "DestThreadedBuffer.h"
#include "DestPgCopy.h"
#include "AppLog.h"

class PostgresMakeTablePriv 
{
    private:
    friend class PostgresMakeTable ;
    RelationPtr             source_;
    DBConnectionPtr         dbconn_;
    char_ptr                postload_sql_ ;
    char_ptr                table_ ;
    size_t                  columns_;

    pDest                   pgdest_ ;
    pDest                   bufdest_ ;

    void                    runSql( const char *s );
};

void
PostgresMakeTablePriv::runSql( const char *sql )
{
    //fprintf(stderr, "exec sql: %s\n", sql );
    RunnablePtr r = new SQLRunnable( dbconn_, sql);
    r->run();
}

PostgresMakeTable::PostgresMakeTable(
        RelationPtr &source,
        DBConnectionPtr &dbconn,
        ConfigPtr &buffer_cfg,
        const char *column_types,
        const char *table_name,
        const char *postload_sql
        )
{
    d_=new PostgresMakeTablePriv ;
    d_->source_ = source;
    d_->dbconn_ = dbconn;

    d_->postload_sql_ = ptr_strdup_wnull(postload_sql);

    if( ! table_name )
    {
        table_name = source->getRelationName();
    }
    d_->table_ =        ptr_strdup(table_name);
    d_->columns_ =      d_->source_->getNumColumns();

    if( !column_types )
    {
        column_types = " varchar(512) ";
    }
    string sql = "create table ";
    sql += table_name ;
    sql += " ( ";
    sql += source->getColumnName(0);
    sql += column_types;
    for( unsigned int c=1; c< source->getNumColumns(); ++c )
    {
        sql += ", ";
        sql += source->getColumnName(c);
        sql += column_types;
    }
    sql += " ) ";
    APPLOG_DBG( sql.c_str());
    d_->runSql( sql.c_str());

    d_->pgdest_ = new DestPgCopy( dbconn, 0, table_name );
    //d_->bufdest_ = DestThreadedBuffer::factory( d_->pgdest_, buffer_cfg );
    d_->bufdest_ = d_->pgdest_ ;
    source->registerListener( this );
}

PostgresMakeTable::~PostgresMakeTable()
{
    delete d_;
}

void PostgresMakeTable::notify()
{
    static int dbg =0;
    ++dbg ;
    size_t i=0 ;
    d_->bufdest_->put(d_->source_->getRaw(i));
    //fprintf(stderr, "notify():%i %i %s\n", dbg, i, d_->source_->getRaw(i));
    for( ++i ; i< d_->columns_ ; ++i )
    {
        d_->bufdest_->put("|");
        d_->bufdest_->put(d_->source_->getRaw(i));
        //fprintf(stderr, "notify():%i %i %s\n", dbg, i, d_->source_->getRaw(i));
    }
    d_->bufdest_->put("\n");
}

void PostgresMakeTable::notifyLast()
{
    //fprintf(stderr, "notifyLast()\n" );
    d_->bufdest_->flush();
    d_->bufdest_ = NULL;//forces flush through buffer and destpgcopy
    (dynamic_cast<DestPgCopy *>(d_->pgdest_.get()))->finish();
    const char *sql = d_->postload_sql_.get() ;
    if( sql && *sql )
    {
        d_->runSql( sql );
    }
}

#endif /* FRAME_USES_POSTGRES */
