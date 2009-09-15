
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      OracleConnection.h
// Author:    Lance Ball
// Created:   Mon Oct 11 14:38:07 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: OracleConnection.h,v 1.6 2004/01/25 04:32:48 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ORACLECONNECTION_H
#define ORACLECONNECTION_H 1

#ifdef HAVE_OCI_H

#include <string>
#include "oci.h"
#include "ptr.h"
#include "db/DBConnection.h"

#define MSG_SZ 512

class OracleConnection : public DBConnection {
    public:
    OracleConnection( const char* user, const char* pass, const char* db );
    virtual const char* errorMessage();
    virtual const char* connectString() const;
    virtual StatementPtr createStatement();
    virtual PreparedStatementPtr prepareStatement(const char *);
    virtual void close();
    virtual bool isClosed();

    int getErrorCode () const;
    bool checkErr( text *_errmsg, dvoid *errhp,
                   sword status, ub4 typ = OCI_HTYPE_ERROR);

    virtual ~OracleConnection();

    OCIError*  getErrorHandle();
    OCIEnv*    getEnvironmentHandle();
    OCISvcCtx* getServiceContext();

    protected:
    bool _checkErr(dvoid *errhp, sword status, ub4 typ = OCI_HTYPE_ERROR);
    bool _closed;
    string _connectStr;
    OCIEnv    *_envhp;
    OCIError  *_errhp;
    OCIServer *_srvhp;
    OCISvcCtx *_svchp;
    text      _errmsg[MSG_SZ];

    private:
    int _errorCode;
};

typedef ptr<OracleConnection> OracleConnPtr;

#endif /* HAVE_OCI_H */
#endif /* ORACLECONNECTION_H */

