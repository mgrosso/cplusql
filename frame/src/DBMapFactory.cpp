#ident "file_id $Id: DBMapFactory.cpp,v 1.1 2004/01/16 21:22:08 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBMapFactory.cpp
// Author:    Lance Ball
// Created:   Fri Oct  1 11:26:53 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBMapFactory.cpp,v 1.1 2004/01/16 21:22:08 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "db/DBMapFactory.h"
#include "db/DBMap.h"
#include "db/DBException.h"
#include "CopyCacheDSMap.h"

#define QUERY_LEN_MAX 1024

DBMapFactory::~DBMapFactory()
{
	//noop
}

DBMapFactory::DBMapFactory(RelationMetaPtr & rel, const char * querytemplate)
: rel_(rel)
{
	query_template_ = ptr_strdup(querytemplate);
}

AbstractDelimitedSourceMapPtr
DBMapFactory::createMap( ConnectionPtr & conn, const char * bucket)
{
	char query[QUERY_LEN_MAX];
	memset(query, '\0', QUERY_LEN_MAX);
	snprintf(query, QUERY_LEN_MAX, query_template_.get(), bucket);
	DBConnectionPtr dconn(conn);
	AbstractDelimitedSourceMapPtr adsm(new DBMap ( rel_, dconn, query ));
	return new CopyCacheDSMap(adsm);
}
