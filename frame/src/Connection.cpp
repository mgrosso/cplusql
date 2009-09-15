#ident "file_id $Id: Connection.cpp,v 1.1 2004/01/16 21:22:40 djain Exp $"

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Connection.cpp
// Author:    Lance Ball
// Created:   Fri Oct  1 13:25:10 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: Connection.cpp,v 1.1 2004/01/16 21:22:40 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Connection.h"

Connection::Connection() {
}

Connection::~Connection() {
    close();
}

void
Connection::close()
{
	//noop
}
