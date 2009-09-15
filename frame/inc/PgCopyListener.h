
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PgCopyListener.h
// Author:    mgrosso 
// Created:   Sat Nov 27 19:02:56 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: PgCopyListener.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PGCOPYLISTENER_H
#define PGCOPYLISTENER_H 1

#include "Listener.h"
#include "AbstractRelation.h"
#include "db/DBConnection.h"
#include "Config.h"
    
class PgCopyListenerPriv ;
class PgCopyListener : public Listener
{
    public:
    PgCopyListener(
        RelationPtr &source,
        DBConnectionPtr &dbconn,
        ConfigPtr &buffer_cfg,
        const char *table_name,
        const char *delim=NULL,//"|"
        const char *preload_sql=NULL,
        const char *postload_sql=NULL
    );
    virtual ~PgCopyListener();
    virtual void notify();
    virtual void notifyLast();

    private:
    PgCopyListenerPriv *d_;
    //not implemented
    PgCopyListener();
    PgCopyListener( const PgCopyListener &rhs );
    PgCopyListener &operator=( const PgCopyListener &rhs );
};



#endif /* PGCOPYLISTENER_H */
