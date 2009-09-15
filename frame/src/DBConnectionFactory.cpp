#ident "file_id $Id: DBConnectionFactory.cpp,v 1.7 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBConnectionFactory.cpp
// Author:    Lance Ball
// Created:   Fri Oct  1 11:26:53 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBConnectionFactory.cpp,v 1.7 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "frame_config.h"

#include <string.h>
#include "db/DBConnectionFactory.h"
#include "db/DBConnection.h"
#include "db/DBException.h"
#include "BaseException.h"
#include "ptr.h"

#ifdef FRAME_USES_ORACLE
#include "db/OracleConnection.h"
#endif
                                                                                
#ifdef FRAME_USES_POSTGRES
#include "db/PostgresConnection.h"
#endif

ConnectionPtr
DBConnectionFactory::createConnection( AuthPtr & auth )
{
	ConnectionPtr conn;
	int db_type = (auth->findString(DBCONNECTION_TYPE) ? auth->getInt(DBCONNECTION_TYPE) : DBConnectionFactory::ORACLE_CONNECTION);

	//fprintf(stderr, "Using user[%s] password [%s] url [%s]\n", auth->getUser(), auth->getPassword(), auth->getUrl());
	switch(db_type)
	{
#ifdef FRAME_USES_ORACLE
		case ORACLE_CONNECTION:
			conn = new OracleConnection(auth->getUser(),
										auth->getPassword(),
										auth->getUrl());
			break;
#endif
#ifdef FRAME_USES_POSTGRES
		case POSTGRES_CONNECTION:
			conn = new PostgresConnection(auth->getUser(),
										auth->getPassword(),
										auth->getUrl());
			break;
#endif
		default:
			PANICV("bad connection type %i, did you ./configure in the type you need?", db_type );
	}
	return conn;
}

ConnectionPtr
DBConnectionFactory::createConnection( const char * user,
										const char * pass,
										const char * url,
										int db_type)
{
	ConnectionPtr conn;

	//fprintf(stderr, "Using user[%s] password [%s] url [%s]\n", auth->getUser(), auth->getPassword(), auth->getUrl());
	switch(db_type)
	{
#ifdef FRAME_USES_ORACLE
		case ORACLE_CONNECTION:
			conn = new OracleConnection(user,
										pass,
										url);
			break;
#endif
#ifdef FRAME_USES_POSTGRES
		case POSTGRES_CONNECTION:
			conn = new PostgresConnection(user,
										pass,
										url);
			break;
#endif
		default:
			PANICV("bad connection type %i, did you ./configure in the type you need?", db_type );
	}
	return conn;
}
