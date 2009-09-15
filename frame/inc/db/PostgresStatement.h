// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresStatement.h
// Author:    djain 
// Created:   Fri Feb  8 09:59:15 PST 2002 on sf-devdw020.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresStatement.h,v 1.1 2002/02/13 23:54:12 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef POSTGRESSTATEMENT_H
#define POSTGRESSTATEMENT_H 1

#include "libpq-fe.h"
#include "Log.h"
#include "db/DBStatement.h"
#include "db/PostgresConnection.h"

class PostgresConnection;

class PostgresStatement : public DBStatement
{
    public:
    virtual ResultSetPtr executeQuery(const char* sql);
    virtual int executeUpdate(const char* sql);
    virtual int getErrorCode () const;
    PostgresConnection * getConnection();

    protected:
    PostgresStatement(PostgresConnection*);
    virtual ~PostgresStatement();

    private:
    PostgresStatement(); // Not Implemented
    PostgresConnection *_parent;
    string _error;
    bool _verbose;

    friend class PostgresConnection;
};

#endif /* POSTGRESSTATEMENT_H */
