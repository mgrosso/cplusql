#ident "file_id $Id: DBConnection.cpp,v 1.9 2004/01/16 21:22:08 djain Exp $"

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBConnection.cpp
// Author:    Lance Ball
// Created:   Fri Oct  1 13:25:10 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBConnection.cpp,v 1.9 2004/01/16 21:22:08 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "db/DBConnection.h"

DBConnection::~DBConnection()
{
	close();
}
