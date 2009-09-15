#ident "file_id $Id: Listener.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Listener.cpp
// Author:    djain Deepak Jain
// Created:   Wed Feb  5 16:43:36 PST 2003 on sf-devdw002.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: Listener.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Listener.h"
#include <stdio.h>

Listener::Listener()
{
}

Listener::~Listener()
{
    //fprintf(stderr, "~Listener only %x\n", (unsigned )this);
}

void
Listener::notifyLast()
{
    ;//noop
}
