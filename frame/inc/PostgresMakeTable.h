
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresMakeTable.h
// Author:    mgrosso 
// Created:   Thu May 27 23:28:13 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: PostgresMakeTable.h,v 1.3 2004/12/31 21:08:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef POSTGRESMAKETABLE_H
#define POSTGRESMAKETABLE_H 1

#include "Listener.h"
#include "AbstractRelation.h"
#include "db/DBConnection.h"
#include "Config.h"


class PostgresMakeTablePriv ;
class PostgresMakeTable : public Listener
{
    public:
    PostgresMakeTable( 
        RelationPtr &source, 
        DBConnectionPtr &dbconn,
        ConfigPtr &buffer_cfg,
        const char *postload_sql=NULL,
        const char *column_types=NULL,  //defaults to varchar
        const char *table_name=NULL     //defaults to relation name
        );
    virtual ~PostgresMakeTable();
    virtual void notify();
    virtual void notifyLast();

    private:
    PostgresMakeTablePriv *d_;
    //not implemented
    PostgresMakeTable();
    PostgresMakeTable( const PostgresMakeTable &rhs );
    PostgresMakeTable &operator=( const PostgresMakeTable &rhs );
};

#endif /* POSTGRESMAKETABLE_H */
