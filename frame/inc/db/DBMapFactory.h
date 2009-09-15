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
// $Id: DBMapFactory.h,v 1.1 2004/01/16 20:58:49 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBMAPFACTORY_H
#define DBMAPFACTORY_H 1

#include "ADSMapFactory.h"
#include "RelationMeta.h"

class DBMapFactory: public ADSMapFactory
{
    public:
	DBMapFactory(RelationMetaPtr& rel, const char * querytemplate);
	virtual ~DBMapFactory();
    virtual AbstractDelimitedSourceMapPtr createMap(ConnectionPtr & conn, const char * bucket);

	private:
	RelationMetaPtr rel_;
	char_ptr query_template_;
	DBMapFactory();
	DBMapFactory( const DBMapFactory &rhs);
    DBMapFactory &operator=( const DBMapFactory &rhs);
};

#endif /* DBMAPFACTORY_H */
