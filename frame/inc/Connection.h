
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBConnection.h
// Author:    Lance Ball
// Created:   Fri Oct  1 13:14:00 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: Connection.h,v 1.1 2004/01/16 21:01:33 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef CONNECTION_H
#define CONNECTION_H 1

#include "ptr.h"

class Connection
{
    public:

	//pure-virtual functions
    virtual const char* errorMessage() = 0;
    virtual const char* connectString() const = 0;
    virtual bool isClosed() = 0;
    virtual int getErrorCode () const = 0;

    virtual void close();
    virtual ~Connection();

	protected:
	Connection();
};

typedef ptr< Connection > ConnectionPtr;

#endif /* CONNECTION_H */
