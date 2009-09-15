// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBDelimitedSource.cpp
// Author:    mgrosso 
// Created:   Sun Apr 27 18:15:05 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DBDelimitedSourcePriv.h,v 1.1 2005/01/24 19:09:40 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBDELIMITEDSOURCEPRIV_H
#define DBDELIMITEDSOURCEPRIV_H 1

#include "db/DBConnection.h"
#include "db/DBStatement.h"
#include "db/DBResultSet.h"

class DBDelimitedSourcePriv 
{
    public:
    friend class DBDelimitedSource ;

	DBDelimitedSourcePriv();
	DBDelimitedSourcePriv( StatementPtr & stmt, ResultSetPtr &rsp);

    StatementPtr stmt_;
    ResultSetPtr result_;
};

#endif  /* DBDELIMITEDSOURCEPRIV_H */
