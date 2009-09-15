#ident "file_id $Id: sequenceWrapper.cpp,v 1.4 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      sequenceWrapper.cpp
// Author:    Lance Ball
// Created:   Thu Sep 30 16:24:30 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: sequenceWrapper.cpp,v 1.4 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "hashFuncs.h"
#include "db/sequenceWrapper.h"
#include "db/DBException.h"
#include "db/DBConnection.h"

string_to_sequenceWrapper_hash sequenceWrapper::_selves;

sequenceWrapper *
sequenceWrapper::Instance( DBConnectionPtr conn, 
                           const char* sequenceName, 
                           long increment=1 ) 
{
    string connectString = conn->connectString();
    string sequence = string(sequenceName);

    string hashString = connectString + sequence;
    if (_selves[hashString] == NULL)
    {
        _selves[hashString] = new sequenceWrapper(conn, sequenceName, increment);
    }
    return _selves[hashString];
}    

sequenceWrapper::~sequenceWrapper()
{
}

sequenceWrapper::sequenceWrapper( DBConnectionPtr conn, 
                                  const char* sequenceName, 
                                  long increment=1 ) 
: _increment_by(increment), _conn(conn), _stmt(_conn->createStatement())
{
    assert(sequenceName);
    _seqName = sequenceName;
    _select  = "SELECT " + _seqName + ".nextval FROM dual";
    _getNewBaseNum();
}

void
sequenceWrapper::close() 
{
    _conn->close();
}

long long
sequenceWrapper::nextval() 
{
    if (++_currval == (_baseval + _increment_by)) {
        _getNewBaseNum();
    }
    return _currval;
}

long long
sequenceWrapper::currval() 
{
    return _currval;
}

void
sequenceWrapper::_getNewBaseNum() 
{
    ResultSetPtr rs = _stmt->executeQuery(_select.c_str());
    rs->next();
    string sval = rs->getString(0);
    if (!sval.c_str()) {
        _currval = -1;
    }

    if (sscanf(sval.c_str(), "%Li", &_currval) != 1)
    {
        PANICV("Cannot convert sequence from string to long!");
    }
    --_currval;
    _baseval = _currval;
}
