// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBPreparedStatement.h
// Author:    djain Deepak Jain,,,,mgrosso,00000000
// Created:   Fri Feb 22 08:10:24 PST 2002 on devbidb.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DBPreparedStatement.h,v 1.2 2002/05/08 04:56:07 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBPREPAREDSTATEMENT_H
#define DBPREPAREDSTATEMENT_H 1

#include "ptr.h"
#include "db/DBStatement.h"

class DBPreparedStatement : public DBStatement{
    public:
    virtual ResultSetPtr executeQuery() = 0;
    virtual int executeUpdate() = 0;
    virtual void setString(int paramIndex, const char*) = 0;
    virtual void setLong(int paramIndex, long) = 0;
    virtual void setNumeric(int paramIndex, long long) = 0;
    virtual void setInt(int paramIndex, int) = 0;
    virtual ~DBPreparedStatement();

    protected:
    DBPreparedStatement();
};

typedef ptr< DBPreparedStatement > PreparedStatementPtr;

#endif /* DBSPREPAREDTATEMENT_H */

