
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      sequenceWrapper.h
// Author:    Lance Ball
// Created:   Thu Sep 30 16:18:35 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: sequenceWrapper.h,v 1.3 2004/01/16 20:58:49 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SEQUENCEWRAPPER_H
#define SEQUENCEWRAPPER_H 1

#include <string>
#include <db/DBStatement.h>
#include <db/DBConnection.h>
#include "hashFuncs.h"

class sequenceWrapper;

typedef hash_map<string, sequenceWrapper *, string_hash, strings_equal> string_to_sequenceWrapper_hash;

class sequenceWrapper 
{
  public:
    static sequenceWrapper * Instance( DBConnectionPtr conn,  
                                      const char* sequenceName,
                                      long increment );

    long long nextval();
    long long currval();
    void close();
    ~sequenceWrapper();

  private:
    sequenceWrapper( DBConnectionPtr conn,  
                     const char* sequenceName, 
                     long increment );

    void _getNewBaseNum();
    long long _baseval;
    long long _currval;
    long _increment_by;
    static string_to_sequenceWrapper_hash _selves;

    string _seqName;
    string _select;

    DBConnectionPtr _conn;
    StatementPtr _stmt;
};

#endif /* SEQUENCEWRAPPER_H */

