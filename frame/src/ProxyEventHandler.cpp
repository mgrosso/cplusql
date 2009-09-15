#ident "file_id $Id: ProxyEventHandler.cpp,v 1.1 2004/01/25 02:24:53 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyEventHandler.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Wed Jan 21 20:15:06 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ProxyEventHandler.cpp,v 1.1 2004/01/25 02:24:53 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ProxyEventHandler.h"

ProxyEventHandler::ProxyEventHandler( EventHandler *h )
:h(h)
{
    ;//noop
}

ProxyEventHandler::~ProxyEventHandler()
{
    ;//noop
}

void 
ProxyEventHandler::handleEvent( void *e )
{
    h->handleEvent(e);
}
