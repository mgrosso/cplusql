#ident "file_id $Id: AbstractDelimitedSource.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractDelimitedSource.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Fri Sep  6 00:26:32 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractDelimitedSource.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "AbstractDelimitedSource.h"
#include <stdio.h>

AbstractDelimitedSource::AbstractDelimitedSource()
{
    //noop
}
AbstractDelimitedSource::~AbstractDelimitedSource()
{
    //fprintf(stderr, "~AbstractDelimitedSource only %x\n", (unsigned )this);
    //noop
}

