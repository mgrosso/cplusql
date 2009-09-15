#ident "file_id $Id: OracleConnection.cpp,v 1.9 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      OracleConnection.cpp
// Author:    Lance Ball
// Created:   Mon Oct 11 14:48:56 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: OracleConnection.cpp,v 1.9 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifdef HAVE_OCI_H


#include <iostream>
#include <string.h>
#include "db/OracleConnection.h"
#include "db/OracleStatement.h"
#include "BaseException.h"

OCIError*
OracleConnection::getErrorHandle()
{
    return _errhp;
}

OCIEnv*
OracleConnection::getEnvironmentHandle()
{
    return _envhp;
}

OCISvcCtx*
OracleConnection::getServiceContext()
{
    return _svchp;
}

OracleConnection::OracleConnection( const char* user, 
                                    const char* pass, 
                                    const char* db ) {
    _closed = true;
    _connectStr = user + string("/") + pass + string("@") + db;
    memset(_errmsg, 0, MSG_SZ);
    // Initialize our oci process
    if (OCIInitialize( (ub4) OCI_DEFAULT, (dvoid *)0,
                       (dvoid * (*)(dvoid *, size_t)) 0,
                       (dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
                       (void (*) (dvoid *, dvoid *)) 0 ) == OCI_ERROR ) {
        cerr << "Cannot initialize Oracle OCI" << endl;
        PANICV ("Cannot initialize ORACLE OCI");
        return;
    }

    // Initialize our environment
    if (OCIEnvInit( (OCIEnv **) &_envhp, OCI_DEFAULT, (size_t) 0,
                    (dvoid **) 0 ) == OCI_ERROR ) {
        cerr << "Cannot initialize DB Environment" << endl;
        _checkErr((dvoid*)_envhp, OCI_ERROR, OCI_HTYPE_ENV); 
        PANICV ("Cannot initialize ORACLE OCI ENVIRONMENT");
        return;
    }

    // Allocate all of our handles.
    if (OCIHandleAlloc( (dvoid *) _envhp, (dvoid **)&_errhp,OCI_HTYPE_ERROR,
                        (size_t) 0, (dvoid **) 0 ) == OCI_ERROR ) {
        cerr << "Cannot initialize Error Handle" << endl;
        _checkErr((dvoid*)_envhp, OCI_ERROR, OCI_HTYPE_ENV); 
        PANICV ("Cannot initialize ORACLE Error Handle");
        return;
    }
    if (OCIHandleAlloc( (dvoid *) _envhp, (dvoid **)&_srvhp,OCI_HTYPE_SERVER,
                        (size_t) 0, (dvoid **) 0 ) == OCI_ERROR ) {
        cerr << "Cannot initialize Server Handle" << endl;
        _checkErr((dvoid*)_envhp, OCI_ERROR, OCI_HTYPE_ENV); 
        PANICV ("Cannot initialize ORACLE Server handle");
        return;
    }
    if (OCIHandleAlloc( (dvoid *) _envhp, (dvoid **)&_svchp,OCI_HTYPE_SVCCTX,
                        (size_t) 0, (dvoid **) 0 ) == OCI_ERROR ) {
        cerr << "Cannot initialize Context Handle" << endl;
        _checkErr((dvoid*)_envhp, OCI_ERROR, OCI_HTYPE_ENV); 
        PANICV ("Cannot initialize ORACLE Context Handle");
        return;
    }

    // Logon to the DB
    if (!_checkErr((dvoid*)_errhp, OCILogon( _envhp, _errhp, &_svchp, 
                                     (text *)user, 
                                     strlen(user), (text *)pass,
                                     strlen(pass), (text *)db, 
                                     strlen(db) )) ) {

        PANICV( "Cannot login to DB as %s/%s@%s", 
            user, "password-hidden", db );
    }
    _closed = false;
}

StatementPtr
OracleConnection::createStatement() {
    StatementPtr stmt( new OracleStatement(this) );
    return stmt;
}

PreparedStatementPtr
OracleConnection::prepareStatement(const char * sql) {
    /* No prepared statement implementation
     * for Oracle db driver yet
     */
    PANICV("Unsupported Exception.");
}


void
OracleConnection::close() {
    if (_closed) { return; }
    OCILogoff( _svchp, _errhp );
    if (_errhp) {
        OCIHandleFree( (dvoid *) _errhp, OCI_HTYPE_ERROR );
    }
    _closed = true;
}

bool
OracleConnection::isClosed() {
    return _closed;
}

const char*
OracleConnection::connectString() const {
    return _connectStr.c_str();
}

const char*
OracleConnection::errorMessage() {
    return (const char*)_errmsg;
}

OracleConnection::~OracleConnection() {
    this->close();
}

bool
OracleConnection::checkErr( text *msg, dvoid *errhp, sword status,  
                            ub4 typ ) {
    sb4  errcode = 0;
    bool retval = false;

    switch (status) {
        case OCI_SUCCESS:
            retval = true;
            break;
        case OCI_SUCCESS_WITH_INFO:
            memset((char *)msg, 0, MSG_SZ);
            strncpy((char *)msg, "Error - OCI_SUCCESS_WITH_INFO", MSG_SZ-1);
            retval = true;
            break;
        case OCI_NEED_DATA:
            memset((char *)msg, 0, MSG_SZ);
            strncpy((char *)msg, "Error - OCI_NEED_DATA", MSG_SZ-1);
            break;
        case OCI_NO_DATA:
            memset((char *)msg, 0, MSG_SZ);
            strncpy((char *)msg, "Error - OCI_NO_DATA", MSG_SZ-1);
            break;
        case OCI_ERROR:
            char codestr[8];
            memset((char *)codestr, 0, 8);
            memset((char *)msg, 0, MSG_SZ);
            OCIErrorGet((dvoid *)errhp, (ub4) 1, (text*) NULL, &errcode,
                        msg, (ub4) sizeof(msg), typ);
            sprintf((char *)codestr, "%d", errcode);
            strncat((char *)msg, " - ", MSG_SZ-strlen((char *)msg));
            strncat((char *)msg, codestr, MSG_SZ-strlen((char *)msg));
            break;
        case OCI_INVALID_HANDLE:
            memset((char *)msg, 0, MSG_SZ);
            strncpy((char *)msg, "Error - OCI_INVALID_HANDLE", MSG_SZ-1);
            break;
        case OCI_STILL_EXECUTING:
            memset((char *)msg, 0, MSG_SZ);
            strncpy((char *)msg, "Error - OCI_STILL_EXECUTING", MSG_SZ-1);
            break;
        case OCI_CONTINUE:
            memset((char *)msg, 0, MSG_SZ);
            strncpy((char *)msg, "Error - OCI_CONTINUE", MSG_SZ-1);
            retval = true;
            break;
        default:
            retval = true;
            break;
    }

    _errorCode = errcode;

    return retval;
}

bool
OracleConnection::_checkErr( dvoid *errhp, sword status,  
                             ub4 typ ) {
    return OracleConnection::checkErr(_errmsg, errhp, status, typ);
}

int OracleConnection::getErrorCode () const
{
  return _errorCode;
}

#endif /*  HAVE_OCI_H */
