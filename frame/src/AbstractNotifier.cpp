#ident "file_id $Id: AbstractNotifier.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractNotifier.cpp
// Author:    mgrosso 
// Created:   Tue Aug 19 10:19:46 EDT 2003 on dhcp-172-18-100-101.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: AbstractNotifier.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AbstractNotifier.h"
#include <stdio.h>

    

AbstractNotifier::AbstractNotifier()
{
    ;//noop
}

AbstractNotifier::~AbstractNotifier()
{
    //fprintf(stderr, "~AbstractNotifier only %x\n", (unsigned )this);
}

AbstractNotifier::AbstractNotifier( const AbstractNotifier &rhs )
{
    ;//noop
}

AbstractNotifier &
AbstractNotifier::operator = ( const AbstractNotifier &rhs )
{
    return *this;
}
