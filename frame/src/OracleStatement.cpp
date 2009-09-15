#ident "file_id $Id: OracleStatement.cpp,v 1.9 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      OracleStatement.cpp
// Author:    Lance Ball
// Created:   Mon Oct 11 16:31:54 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: OracleStatement.cpp,v 1.9 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifdef HAVE_OCI_H

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "db/OracleStatement.h"
#include "db/OracleResultSet.h"
#include "db/OracleConnection.h"
#include "db/DBException.h"
#include "Convert.h"

OCIError*
OracleStatement::getErrorHandle()
{
    return _parent->getErrorHandle();
}

OCIEnv*
OracleStatement::getEnvironmentHandle()
{
    return _parent->getEnvironmentHandle();
}

OCIStmt*
OracleStatement::getStatementHandle()
{
    return _stmthp;
}

OCISvcCtx*
OracleStatement::getServiceContext()
{
    return _parent->getServiceContext();
}

ResultSetPtr
OracleStatement::executeQuery( const char* sql ) {
    sword status;
    status = OCIStmtPrepare( _stmthp, getErrorHandle(), (text*) sql, 
                            (ub4) strlen(sql), 
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT );
    if (! _parent->checkErr( (text*)_errmsg, 
                                      (dvoid*)getErrorHandle(),
                                      status )) {
        PANICV("ERR: OracleStatememt::executeQuery: OCIStmtPrepare: sql(%s) err(%s)",
            sql, _errmsg );
    }

    //Set prefetch row size
    ub4 cache_rows = (getenv("DB_RESULT_PREFETCH_ROWS") == NULL) ? 100000 : Convert::convertToUnsigned(getenv("DB_RESULT_PREFETCH_ROWS"));
    status = OCIAttrSet(_stmthp, OCI_HTYPE_STMT, &cache_rows, sizeof(cache_rows), OCI_ATTR_PREFETCH_ROWS, getErrorHandle());
    if (! _parent->checkErr( (text*)_errmsg, 
                                      (dvoid*)getErrorHandle(),
                                      status )) {
        PANICV( "ERR: OracleStatement::executeUpdate: OCIStmtPrepare: sql(%s) err(%s)", sql, _errmsg );
    }

    //Set prefetch memory size(in KB)
    ub4 cache_mem = (getenv("DB_RESULT_PREFETCH_MEMORY") == NULL) ? 1024 * 1024 : Convert::convertToUnsigned(getenv("DB_RESULT_PREFETCH_MEMORY")) * 1024;
    status = OCIAttrSet(_stmthp, OCI_HTYPE_STMT, &cache_mem, sizeof(cache_mem), OCI_ATTR_PREFETCH_MEMORY, getErrorHandle());
    if (! _parent->checkErr( (text*)_errmsg, 
                                      (dvoid*)getErrorHandle(),
                                      status )) {
        PANICV("ERR: OracleStatement::executeUpdate: OCIStmtPrepare: sql(%s) err(%s)", sql, _errmsg);
        return NULL;
    }

    status = OCIStmtExecute( getServiceContext(), _stmthp, 
                             getErrorHandle(), 
                             (ub4) 0, (ub4) 0, (CONST OCISnapshot *) NULL, 
                             (OCISnapshot *) NULL, OCI_DEFAULT ); 
    if (! _parent->checkErr( (text*)_errmsg, 
                                      (dvoid*)getErrorHandle(),
                                      status )) {
        PANICV("ERR: OracleStatement::executeQuery: OCIStmtExecute: sql(%s) err(%s)", sql, _errmsg);
    }

    if (_verbose)
    {
        //cerr << "OracleStatement::executeQuery(): " << sql << endl;
    }
    ResultSetPtr rs( new OracleResultSet(this) )  ;
    return rs;
}

int
OracleStatement::executeUpdate( const char* sql ) {
    sword status;
    ub4 numRows = 0;
    status = OCIStmtPrepare( _stmthp, getErrorHandle(), (text*) sql, 
                            (ub4) strlen(sql), 
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT );
    if (! _parent->checkErr( (text*)_errmsg, 
                                      (dvoid*)getErrorHandle(),
                                      status )) {
        PANICV("ERR: OracleStatement::executeUpdate: OCIStmtPrepare: sql(%s) err(%s)", sql, _errmsg);
    }

    status = OCIStmtExecute( getServiceContext(), _stmthp, 
                             getErrorHandle(), 
                             (ub4) 1, (ub4) 0, (CONST OCISnapshot *) NULL, 
                             (OCISnapshot *) NULL, OCI_COMMIT_ON_SUCCESS ); 
    if (! _parent->checkErr( (text*)_errmsg, 
                                      (dvoid*)getErrorHandle(),
                                      status )) {
        PANICV("ERR: OracleStatement::executeUpdate: OCIStmtExecute: sql(%s) err(%s)", sql, _errmsg);
    }
    OCIAttrGet( (CONST dvoid*) _stmthp, OCI_HTYPE_STMT, &numRows, (ub4*) NULL,
                OCI_ATTR_ROW_COUNT, getErrorHandle() );

    //if (_verbose)
    //{
        //cerr << "OracleStatement::executeUpdate(): " << sql 
        //     << " [" << __LINE__ << "]" 
        //     << "[" << __FILE__ << "]" << endl;
    //}
    return numRows;
}

OracleStatement::OracleStatement() {
    PANICV("INVALID STATEMENT INSTANCE");
}

OracleStatement::OracleStatement(OracleConnection* parent) : _parent(parent) {

    if (! _parent->checkErr( (text *)_errmsg, 
                        (dvoid*)getEnvironmentHandle(),
                        OCIHandleAlloc((dvoid*)getEnvironmentHandle(), 
                        (dvoid **)&_stmthp, OCI_HTYPE_STMT, (size_t) 0, 
                        (dvoid **) 0 ), OCI_HTYPE_ENV )) {
        PANICV("ERR: OracleStatement::OracleStatement(): Cannot initialize statement Handle: %s", _errmsg );
    }
    _verbose = (getenv("SQL_DEBUG") == NULL) ? false : true;
}

OracleStatement::~OracleStatement() {
    if (_stmthp) {
        sword status;
        status = OCIHandleFree((dvoid *)_stmthp, OCI_HTYPE_STMT);
    }
}

int OracleStatement::getErrorCode () const
{
  return _parent->getErrorCode ();
}

OracleConnection * OracleStatement::getOracleConnectionParent ()
{
  return _parent;
}

#endif /* HAVE_OCI_H */
