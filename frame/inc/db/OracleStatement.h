
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      OracleStatement.h
// Author:    Lance Ball
// Created:   Mon Oct 11 16:03:59 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: OracleStatement.h,v 1.4 2004/01/25 04:32:48 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifdef HAVE_OCI_H
#ifndef ORACLESTATEMENT_H
#define ORACLESTATEMENT_H 1

#include "oci.h"
#include "Log.h"
#include "db/DBStatement.h"
#include "db/OracleConnection.h"

#define ERR_MSG_SIZE 512
class OracleConnection;

class OracleStatement : public DBStatement {
    public:
    virtual int executeUpdate(const char* sql);
    virtual ResultSetPtr executeQuery(const char* sql);

    OCIError*  getErrorHandle();
    OCIEnv*    getEnvironmentHandle();
    OCISvcCtx* getServiceContext();
    OCIStmt*   getStatementHandle();

    virtual int getErrorCode () const;
    OracleConnection * getOracleConnectionParent ();

    protected:
    OracleStatement(OracleConnection*);
    virtual ~OracleStatement();

    private:
    OracleStatement(); // Not Implemented

    OracleConnection *_parent;
    OCIStmt *_stmthp;
    char _errmsg[ERR_MSG_SIZE];
    bool _verbose;

    friend class OracleConnection;
};

#endif /* ORACLESTATEMENT_H */
#endif /* HAVE_OCI_H */

