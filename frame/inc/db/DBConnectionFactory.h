
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBManager.h
// Author:    Lance Ball
// Created:   Thu Sep 30 16:12:36 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBConnectionFactory.h,v 1.4 2004/06/02 21:08:33 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBCONNECTIONFACTORY_H
#define DBCONNECTIONFACTORY_H 1

#include "frame_config.h"
#include "Auth.h"
#include "ConnectionFactory.h"
#include "db/DBException.h"

#define DBCONNECTION_TYPE "DBCONNECTION_TYPE"

class DBConnectionFactory: public ConnectionFactory {
    public:
    virtual ConnectionPtr createConnection(AuthPtr&);
    static ConnectionPtr createConnection(const char * user,
										   const char * pass,
										   const char * url,
										   int db_type);

	static const int ORACLE_CONNECTION = 0,
					 POSTGRES_CONNECTION = 1;

};

#endif /* DBCONNECTIONFACTORY_H */
