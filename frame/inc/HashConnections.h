
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      inc/HashConnections.h
// Author:    djain 
// Created:   Thu Nov  6 16:55:52 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: HashConnections.h,v 1.1 2004/01/16 21:03:11 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef HASHCONNECTIONS_H
#define HASHCONNECTIONS_H 1

#include "Expression.h"
#include "Auth.h"
#include "Connection.h"
#include "ConnectionFactory.h"

class HashConnectionsPriv;

class HashConnections
{
	public:
	HashConnections(ConnectionFactoryPtr&, auth_map_t & auth_map);
	HashConnections(ConnectionFactoryPtr&);
	virtual ConnectionPtr& getConnection(const char * val);
	virtual ConnectionPtr& getConnection(AuthPtr& auth_map, const char * val);
	virtual void addAuth(AuthPtr & auth, const char * val);
	virtual ~HashConnections();

	private:
	HashConnectionsPriv* d_;

	HashConnections();
    HashConnections(const HashConnections &rhs);
    HashConnections &operator= (const HashConnections &rhs);
};

typedef ptr<HashConnections> HashConnectionsPtr;
#endif /* HASHCONNECTIONS_H */
