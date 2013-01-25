#ident "file_id $Id: Auth.cpp,v 1.1 2004/01/16 21:22:40 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Auth.cpp
// Author:    djain 
// Created:   Thu Nov  6 18:05:11 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: Auth.cpp,v 1.1 2004/01/16 21:22:40 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "Auth.h"

Auth::Auth()
:Config()
{
}
	
Auth::Auth(ConfigPtr & config)
:Config()
{
	slurp(config, true);
}

void
Auth::setUser(const char * user)
{
	addString( AUTH_USER, user, true);
}

void
Auth::setPassword(const char * password)
{
	addString( AUTH_PASSWORD, password, true);
}

void
Auth::setUrl(const char * url)
{
	addString( AUTH_URL, url, true);
}

const char *
Auth::getUser()
{
	return findString(AUTH_USER);
}

const char *
Auth::getPassword()
{
	return findString(AUTH_PASSWORD);
}

const char *
Auth::getUrl()
{
	return findString(AUTH_URL);
}

Auth::~Auth()
{
	//;
}
