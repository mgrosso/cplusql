#ident "file_id $Id: PostgresStatement.cpp,v 1.8 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresStatement.cpp
// Author:    djain Deepak Jain,,,,mgrosso,00000000
// Created:   Fri Feb  8 12:20:51 PST 2002 on devbidb.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresStatement.cpp,v 1.8 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifdef FRAME_USES_POSTGRES

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libpq-fe.h>

#include "db/PostgresStatement.h"
#include "db/PostgresResultSet.h"
#include "db/PostgresConnection.h"
#include "db/DBException.h"

PostgresStatement::PostgresStatement(PostgresConnection* parent):
						_parent(parent)
{
}

ResultSetPtr
PostgresStatement::executeQuery( const char* sql )
{
	PGconn * pqconn = _parent->getPostgresConnection();
#ifdef DEBUG
	cerr << "PostgresStatement::executeQuery(): query[" << sql << "]" << endl;
#endif
	PGresult * result = PQexec(pqconn, sql);
	if(!result)
	{
        PANICV("Unexpected error while executing query[%s].  Error:[%s].\n", sql, PQerrorMessage(pqconn));
	}

	ResultSetPtr rs;
	int code = PQresultStatus(result);
    _parent->setErrorCode(0);
    string empty_message;
    _parent->setErrorMessage(empty_message);

	if(code == PGRES_EMPTY_QUERY)
	{
		_parent->setErrorCode(code);
		string mess("Query passed to executeQuery may not be empty.");
		_parent->setErrorMessage(mess);
		PANICV(mess.c_str());
	}
	else if(code == PGRES_COMMAND_OK)
    {
        PQclear(result);
        rs = new PostgresResultSet(this,PQmakeEmptyPGresult(pqconn, PGRES_TUPLES_OK)); 
    }
	else if(code != PGRES_TUPLES_OK)
	{
		_parent->setErrorCode(code);
		string mess(PQresultErrorMessage(result));
		_parent->setErrorMessage(mess);
		PANICV(mess.c_str());
	}
    else{
        rs = new PostgresResultSet(this, result);
    }
#ifdef DEBUG
	cerr << "PostgresStatement::executeQuery(): after query[" << sql << "]" << endl;
#endif
	return rs;
}

int
PostgresStatement::executeUpdate( const char* sql ) {
    //TODO: write template ptr delete to call PQclear on results...
	PGresult * result = PQexec(_parent->getPostgresConnection(), sql);
	if(!result)
	{
		PANICV("Unexpected error while executing query.");
	}
	int code = PQresultStatus(result);
	if(code == PGRES_EMPTY_QUERY)
	{
        PQclear(result);
		PANICV("Query passed to executeQuery may not be empty.");
	}
	else if(code == PGRES_TUPLES_OK)
    {
        PQclear(result);
        return 0;
    }
	else if(code != PGRES_COMMAND_OK)
	{
		_parent->setErrorCode(code);
		string mess(PQresultErrorMessage(result));
		_parent->setErrorMessage(mess);
        PQclear(result);
		PANICV(mess.c_str());
	}
	_parent->setErrorCode(0);
	string empty_message;
	_parent->setErrorMessage(empty_message);
    PQclear(result);

	return atoi(PQcmdTuples( result ));
}

PostgresStatement::~PostgresStatement()
{
#ifdef DEBUG
	cerr << "PostgresStatement::~PostgresStatement() called." << endl;
#endif
}

int PostgresStatement::getErrorCode () const
{
	return _parent->getErrorCode ();
}

PostgresConnection * PostgresStatement::getConnection()
{
	return _parent;
}
#endif /* FRAME_USES_POSTGRES */
