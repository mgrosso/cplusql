
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBConnection.h
// Author:    Lance Ball
// Created:   Fri Oct  1 13:14:00 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBConnection.h,v 1.6 2004/01/16 20:58:48 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBCONNECTION_H
#define DBCONNECTION_H 1

#include "ptr.h"
#include "Connection.h"
#include "db/DBStatement.h"
#include "db/DBPreparedStatement.h"

class DBConnection: public Connection {
    public:
    virtual StatementPtr createStatement() = 0;
    virtual PreparedStatementPtr prepareStatement(const char * sql) = 0;
    virtual ~DBConnection();

    private:
};

typedef ptr< DBConnection > DBConnectionPtr;
#endif /* DBCONNECTION_H */

