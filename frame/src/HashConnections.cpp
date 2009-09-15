#ident "file_id $Id: HashConnections.cpp,v 1.2 2004/01/20 19:52:44 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashConnections.cpp
// Author:    djain 
// Created:   Thu Nov  6 16:55:16 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: HashConnections.cpp,v 1.2 2004/01/20 19:52:44 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "HashConnections.h"

typedef map< const char *, ConnectionPtr, cc_less_than > connection_map_t;

class HashConnectionsPriv
{
	private:
	friend class HashConnections;

	char_ptr_list			bucket_list_;
	ConnectionFactoryPtr	factory_;
	auth_map_t				authm_;
	connection_map_t		conns_;
};

HashConnections::HashConnections(ConnectionFactoryPtr& factory)
{
	d_ = new HashConnectionsPriv();
	d_->factory_ = factory;
}

HashConnections::HashConnections(ConnectionFactoryPtr& factory, auth_map_t & auth_map)
{
	d_ = new HashConnectionsPriv();
	d_->factory_ = factory;
	d_->authm_ = auth_map;
}

void
HashConnections::addAuth(AuthPtr & auth, const char * val)
{
	auth_map_t::iterator authi = d_->authm_.find(val);
	auth_map_t::iterator authe = d_->authm_.end();
	if(authi == authe)
	{
		char_ptr valp = ptr_strdup(val);
		d_->bucket_list_.push_back(valp);
		d_->authm_.insert(pair<const char *, AuthPtr>(valp.get(), auth));
	}
}

ConnectionPtr &
HashConnections::getConnection(AuthPtr & auth, const char * val)
{
	connection_map_t::iterator conni = d_->conns_.find(val);
	connection_map_t::iterator conne = d_->conns_.end();
	if(conni==conne)
	{
		addAuth(auth, val);
	}
	return getConnection(val);
}

ConnectionPtr &
HashConnections::getConnection(const char * val)
{
	connection_map_t::iterator conni = d_->conns_.find(val);
	connection_map_t::iterator conne = d_->conns_.end();
	if(conni==conne)
	{
		auth_map_t::iterator authi = d_->authm_.find(val);
		auth_map_t::iterator authe = d_->authm_.end();
		if(authi == authe)
		{
			throw BaseException::factory("%s:%i: getConnection(): There is no authentication entry for value of %s", __FILE__, __LINE__, val);
		}
		ConnectionPtr conn(d_->factory_->createConnection((*authi).second));
		return (*(d_->conns_.insert(pair<const char *, ConnectionPtr>(val, conn)).first)).second;
	}
	return (*conni).second;
}

HashConnections::~HashConnections()
{
	connection_map_t::iterator conni = d_->conns_.begin();
	connection_map_t::iterator conne = d_->conns_.end();
	while(conni!=conne)
	{
		((*conni).second)->close();
		++conni;
	}
	delete d_;
}
