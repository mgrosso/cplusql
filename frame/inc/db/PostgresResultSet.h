// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresResultSet.h
// Author:    djain 
// Created:   Fri Feb  8 09:59:10 PST 2002 on sf-devdw020.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresResultSet.h,v 1.2 2003/10/25 05:30:44 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef POSTGRESRESULTSET_H
#define POSTGRESRESULTSET_H 1

#include <vector>
#include <libpq-fe.h>
#include <string>
#include "db/DBResultSet.h"

#define RS_MSG_BUF_SZ 512

class PostgresStatement;

struct colMeta {
    string colName;
    int columnLen;
    Oid dataType;
};

typedef vector < colMeta > ORSMetaData;

class PostgresResultSet : public DBResultSet
{
    public:
    virtual bool next();
    virtual DBRowPtr getRow();
    virtual const char * getString(int idx);
    virtual const char * getString(const char * colname);
    virtual int numColumns();
    virtual int rowsProcessed();
    virtual int colLength(int idx);
    virtual int colLength(const char * colname);
    virtual const char * colName(int idx);
    virtual bool isValid();
    virtual int getErrorCode () const;

    protected:
    PostgresResultSet(PostgresStatement*, PGresult*, bool valid=true);
    virtual ~PostgresResultSet();

    private:
    PostgresStatement *_parent;
    PGresult * _result;
    bool _valid;
    char _errmsg[RS_MSG_BUF_SZ];
    int _rowsProcessed;
    ORSMetaData _meta;
    DBRowPtr _currentRow;
    int _num_rows;

    friend class PostgresStatement;
};

#endif /* POSTGRESRESULTSET_H */
