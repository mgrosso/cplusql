#ident "file_id $Id: ProxyRunnable.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyRunnable.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb  9 02:54:43 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ProxyRunnable.cpp,v 1.1 2004/02/12 21:16:46 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "ProxyRunnable.h"

ProxyRunnable::ProxyRunnable( Runnable *r )
:r_(r)
{
    ;//noop 
}

ProxyRunnable::~ProxyRunnable()
{
    ;//noop
}

void
ProxyRunnable::run()
{
    r_->run();
}

