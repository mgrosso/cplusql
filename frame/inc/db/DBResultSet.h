// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBResultSet.h
// Author:    Lance Ball
// Created:   Mon Oct 11 17:19:48 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBResultSet.h,v 1.6 2003/10/25 05:30:44 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBRESULTSET_H
#define DBRESULTSET_H 1

#include "ptr.h"
#include "ptr_strdup.h"
#include <vector>
#include <string>

using namespace std;

typedef char_ptr_vec DBRow;
typedef ptr< DBRow > DBRowPtr;

class DBResultSet {
    public:
    virtual bool next()                             = 0;
    virtual DBRowPtr getRow()                          = 0;
    virtual const char * getString(int idx)               = 0;
    virtual const char * getString(const char * colname) = 0;
    virtual int numColumns()                        = 0;
    virtual int rowsProcessed()                     = 0;
    virtual int colLength(int idx)                  = 0;
    virtual int colLength(const char * colname)    = 0;
    virtual const char * colName(int idx)                 = 0;
    virtual bool isValid()                          = 0;
    virtual int getErrorCode () const               = 0;
    virtual ~DBResultSet();

    protected:
    DBResultSet();
};

typedef ptr< DBResultSet > ResultSetPtr;

#endif /* DBRESULTSET_H */

