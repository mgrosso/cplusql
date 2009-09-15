#ident "file_id $Id: SlurpedLineSource.cpp,v 1.3 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SlurpedLineSource.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 14 20:00:54 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SlurpedLineSource.cpp,v 1.3 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SlurpedLineSource.h"
#include "SlurpedSourcePriv.h"
#include "Parse.h"

SlurpedLineSource::SlurpedLineSource( const char *filename )
{
    init( filename );
}

SlurpedLineSource::~SlurpedLineSource( )
{
}

void 
SlurpedLineSource::do_parse(char *s )
{
    d_->pv_= Parse::parse_ptr(s,"\n");
}

