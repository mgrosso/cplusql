// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresConnection.h
// Author:    djain 
// Created:   Fri Feb  8 09:59:05 PST 2002 on sf-devdw020.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresConnection.h,v 1.6 2005/01/24 19:27:32 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef POSTGRESCONNECTION_H
#define POSTGRESCONNECTION_H 1

#include <string>
#include "libpq-fe.h"
#include "ptr.h"
#include "db/DBConnection.h"
#include "BaseException.h"

class PostgresConnection : public DBConnection
{
    public:
	PostgresConnection( const char* dbuser,
				const char* dbpass,
				const char* dburl);
	virtual const char* errorMessage();
	void setErrorMessage(string &);
	virtual const char* connectString() const;
	virtual StatementPtr createStatement();
	virtual PreparedStatementPtr prepareStatement(const char *);
	virtual void close();
	virtual bool isClosed();
	int getErrorCode () const;
	void setErrorCode (int);
	PGconn * getPostgresConnection();

    PGresult* getResult( ExecStatusType expected );
    PGresult* execute( const char *sql, ExecStatusType expected );
    PGresult* checkResult( PGresult *result, ExecStatusType expected );

	virtual ~PostgresConnection();
	const static int DEFAULT_PORT = 5432;

    protected:

    private:
	PGconn * _conn;
	int _error_code;
	bool _closed;
	string _connectStr;
	string _error_message;
};

typedef ptr<PostgresConnection> PostgresConnPtr;

#define PGPANICIF(c, retwanted, retgot, sqljustrun, errormsg )\
    retwanted != retgot ? \
        PANICV( "PQexec did not return [%i],instead it returned %i. connection errmsg[%s] result errmsg[%s]: fd=%i backend=%i status=%s version=%i callersmsg=[%s] sql=[%s]", \
                    retwanted,  \
                    retgot, \
                    PQerrorMessage( c ), \
                    PQresultErrorMessage( result ), \
                    PQsocket( c ), \
                    PQbackendPID( c ), \
                    CONNECTION_OK == PQstatus ( c ) ? "OK" : "BAD" , \
                    PQprotocolVersion ( c ), \
                    errormsg, \
                    sqljustrun \
                    ) : true ;
 

#define PGPANIC( c, errormsg )\
        PANICV( "Postgres connection problem: connection errmsg[%s] fd=%i backend=%i status=%s version=%i callersmsg=[%s]", \
            PQerrorMessage( c ), \
            PQsocket( c ), \
            PQbackendPID( c ), \
            PQstatus ( c ) == CONNECTION_OK ? "OK" : "BAD" , \
            PQprotocolVersion ( c ), \
            PQerrorMessage( c ) \
            ); 


#endif /* POSTGRESCONNECTION_H */
