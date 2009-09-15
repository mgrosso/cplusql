
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      inc/db/DBException.h
// Author:    djain 
// Created:   Thu Nov  6 18:27:45 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DBException.h,v 1.1 2004/01/16 20:58:49 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H 1

#include "BaseException.h"

class DBException: public BaseException
{
    public:
    DBException();
    DBException(const char*);
    virtual ~DBException() throw();
};

#endif /* DBEXCEPTION_H */
