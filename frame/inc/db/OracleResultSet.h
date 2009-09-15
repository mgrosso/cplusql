
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      OracleResultSet.h
// Author:    Lance Ball
// Created:   Mon Oct 11 17:21:56 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: OracleResultSet.h,v 1.7 2004/01/25 04:32:48 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifdef HAVE_OCI_H
#ifndef ORACLERESULTSET_H
#define ORACLERESULTSET_H 1

#include <vector>
#include <string>
#include "db/DBResultSet.h"

#define RS_MSG_BUF_SZ 512

class OracleStatement;
class OCIParam;

struct colMeta {
    string colName;
    ub4 columnLen;
    ub2 dataType;
};

typedef vector < colMeta > ORSMetaData;

class OracleResultSet : public DBResultSet {
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
    OracleResultSet(OracleStatement*, bool valid=true);
    virtual ~OracleResultSet();

    private:
    OracleStatement *_parent;
    bool _valid;
    char _errmsg[RS_MSG_BUF_SZ];
    int _rowsProcessed;
    ORSMetaData _meta;
    char** _currentRow;
	sword define(ub4 counter, dvoid* val, sb4 len);
	sword fetch();
	void reset();

    friend class OracleStatement;
};

#endif /* ORACLERESULTSET_H */
#endif /* HAVE_OCI_H */

