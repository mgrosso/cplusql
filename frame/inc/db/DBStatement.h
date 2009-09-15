// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBStatement.h
// Author:    Lance Ball
// Created:   Mon Oct 11 14:33:34 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBStatement.h,v 1.4 2004/01/16 20:58:49 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBSTATEMENT_H
#define DBSTATEMENT_H 1

#include "ptr.h"
#include "db/DBResultSet.h"

#ifndef QUERYLEN_MAX
#define QUERYLEN_MAX 2048
#endif

class DBStatement {
    public:
    virtual ResultSetPtr executeQuery( const char* sql ) = 0;
    virtual int executeUpdate( const char* sql ) = 0;
    virtual int getErrorCode () const = 0;
    virtual ~DBStatement();

    protected:
    DBStatement();
};

typedef ptr< DBStatement > StatementPtr;

#endif /* DBSTATEMENT_H */

