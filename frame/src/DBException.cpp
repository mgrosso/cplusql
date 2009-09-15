#ident "file_id $Id: DBException.cpp,v 1.1 2004/01/16 21:22:08 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBException.cpp
// Author:    Lance Ball
// Created:   Fri Oct  1 11:26:53 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: DBException.cpp,v 1.1 2004/01/16 21:22:08 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "db/DBException.h"

DBException::DBException()
:BaseException("DBException!")
{
	;//noop
}

DBException::DBException( const char* s )
:BaseException(s)
{
	;//noop
}

DBException::~DBException() throw()
{
	//;
}
