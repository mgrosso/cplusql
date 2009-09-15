#ident "file_id $Id: CountProjection.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CountProjection.cpp
// Author:    mgrosso 
// Created:   Tue Apr 15 02:32:15 EDT 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CountProjection.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CountProjection.h"

CountProjection::CountProjection()
{
    result_=0;
}

CountProjection::~CountProjection()
{
    ;//noop
}

void
CountProjection::childNotify()
{
    ++result_ ;
}

AbstractProjectionPtr
CountProjection::copy()
{
    AbstractProjectionPtr ap( new CountProjection());
    return ap;
}

