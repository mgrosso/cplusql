
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      inc/Auth.h
// Author:    djain 
// Created:   Thu Nov  6 18:02:10 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: Auth.h,v 1.1 2004/01/16 21:01:34 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef AUTH_H
#define AUTH_H 1

#include "Config.h"
#include "ptr_strdup.h"
#include <map>
#include "hashFuncs.h"

#define AUTH_USER "AUTH_USER"
#define AUTH_PASSWORD "AUTH_PASSWORD"
#define AUTH_URL "AUTH_URL"

class Auth: public Config
{
	public:
	Auth();
	Auth(ConfigPtr &);
	virtual ~Auth();

	const char * getUser();
	const char * getPassword();
	const char * getUrl();
	void setUser(const char *);
	void setPassword(const char *);
	void setUrl(const char *);
	
	private:

    Auth(const Auth &rhs);
    Auth &operator= (const Auth &rhs);
};

typedef ptr<Auth> AuthPtr;
typedef map<const char*, AuthPtr, cc_less_than> auth_map_t;

#endif /* AUTH_H */
