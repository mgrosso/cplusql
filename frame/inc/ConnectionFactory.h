// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ConnectionFactory.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Jan 19 19:28:48 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ConnectionFactory.h,v 1.1 2004/01/16 21:01:33 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CONNECTIONFACTORY_H
#define CONNECTIONFACTORY_H 1

#include "Connection.h"
#include "ptr.h"
#include "Auth.h"


class ConnectionFactory {
    public:
	virtual ConnectionPtr createConnection(AuthPtr&) = 0;

    private:
};

typedef ptr<ConnectionFactory> ConnectionFactoryPtr;

#endif /* CONNECTIONFACTORY_H */
