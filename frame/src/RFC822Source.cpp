#ident "file_id $Id: RFC822Source.cpp,v 1.3 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RFC822Source.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 15 13:30:01 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: RFC822Source.cpp,v 1.3 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RFC822Source.h"
#include <vector>
#include "SlurpedSourcePriv.h"
#include "Parse.h"

RFC822Source::RFC822Source( const char *filename )
{
    init(filename);
}

RFC822Source::~RFC822Source( )
{
}

void 
RFC822Source::do_parse( char *s )
{
    d_->pv_ = Parse::parse_rfc822_ptr( s );
}

