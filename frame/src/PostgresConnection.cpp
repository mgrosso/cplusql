#ident "file_id $Id: PostgresConnection.cpp,v 1.9 2005/04/26 15:14:42 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresConnection.cpp
// Author:    djain 
// Created:   Fri Feb  8 10:23:26 PST 2002 on sf-devdw020.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresConnection.cpp,v 1.9 2005/04/26 15:14:42 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "frame_config.h"
#ifdef FRAME_USES_POSTGRES

#include <iostream>
#include <string.h>
#include "db/PostgresConnection.h"
#include "db/PostgresStatement.h"
#include "db/PostgresPreparedStatement.h"
#include "db/DBException.h"

PostgresConnection::PostgresConnection( const char* dbuser, 
                                    const char* dbpass, 
                                    const char* dburl)
{
    /* First, make sure the url is
     * properly formatted. Format should be:
     *	hostname[:port]/dbname
     */
	if(dburl == NULL)
	{
		PANICV("Url must be set.");
	}
	if(dbuser == NULL)
	{
		PANICV("User must be set.");
	}
    string dbhost, dbname, dbport;
    if(rindex(dburl,'/') == NULL)
    {
	PANICV("Invalid url. Expected: hostname[:port]/dbname");
    }
    if(strcmp(rindex(dburl,'/'), index(dburl,'/')) != 0)
    {
	PANICV("Invalid url. Expected: hostname[:port]/dbname");
    }
    const char * t = rindex(dburl, '/');
    dbname = string(t+1);

    string temp_url(dburl);
    temp_url = temp_url.substr(0, temp_url.length() - (dbname.length() + 1));

    const char * temp_port = strchr(temp_url.c_str(), ':');
    if(temp_port == NULL)
    {
		char tmp_port[5];
		memset(tmp_port, 0, 5);
		/* If no port is specified,
		 * use the default postgres port
		 */
		snprintf(tmp_port, 5, "%i", DEFAULT_PORT);
		dbport = string(tmp_port);
		dbhost = temp_url;
    }
    else
    {
	/* Make sure there is only one : specified
	 */
	if(strcmp(rindex(temp_url.c_str(),':'), index(temp_url.c_str(),':')) != 0)
	{
		PANICV("Invalid url. Expected: hostname[:port]/dbname");
	}
	++temp_port;
	dbport = string(temp_port);
	dbhost = temp_url.substr(0, temp_url.length() - (dbport.length() + 1));
    }

    _error_code = 0;
    _closed = true;
    _connectStr = dbuser + string("/") + dbpass + string("@") + dbhost + string(":") + dbname;

    // Logon to the DB
    _conn = PQsetdbLogin(dbhost.c_str(), dbport.c_str(), NULL, NULL, dbname.c_str(), dbuser, dbpass);
    if(PQstatus(_conn) == CONNECTION_BAD)
    {
        PANICV("Cannot connect to the postgres server on %s:%s, db=%s, user=%s, error=%s",
            dbhost.c_str(), dbport.c_str(), dbname.c_str(), dbuser, PQerrorMessage(_conn));
    }
    _closed = false;
}

StatementPtr
PostgresConnection::createStatement()
{
    StatementPtr stmt( new PostgresStatement(this) );
    return stmt;
}

PreparedStatementPtr
PostgresConnection::prepareStatement(const char * sql)
{
    PreparedStatementPtr pstmt( new PostgresPreparedStatement(this, sql) );
    return pstmt;
}

void
PostgresConnection::close() {
    if (_closed) { return; }
    PQfinish(_conn);
    _closed = true;
}

bool
PostgresConnection::isClosed() {
    return _closed;
}

const char*
PostgresConnection::connectString() const {
    return _connectStr.c_str();
}

const char*
PostgresConnection::errorMessage() {
    return _error_message.c_str();
}

void
PostgresConnection::setErrorMessage(string & message)
{
	_error_message = message;
}

int PostgresConnection::getErrorCode () const
{
  return _error_code;
}

void
PostgresConnection::setErrorCode(int error)
{
	_error_code = error;
}

PGresult *
PostgresConnection::getResult( ExecStatusType expected )
{
    return checkResult( PQgetResult( _conn ), expected );
}

PGresult *
PostgresConnection::execute( const char *sql, ExecStatusType expected )
{
    return checkResult( PQexec( _conn, sql ), expected );
}

PGresult *
PostgresConnection::checkResult(PGresult *result, ExecStatusType expected)
{
    if( ! result )
    {
        PANICV("PQgetResult returned null" );
    }
    ExecStatusType ex = PQresultStatus( result );
    if( expected != ex )
    {
        PANICV("PQgetResult gave wrong answer: %s", 
            PQerrorMessage(_conn));
    }
    return result;
}

PGconn *
PostgresConnection::getPostgresConnection()
{
	return _conn;
}

PostgresConnection::~PostgresConnection()
{
	close();
#ifdef DEBUG
	cerr << "PostgresConnection::~PostgresConnection() called." << endl;
#endif
}

#endif /* FRAME_USES_POSTGRES */
