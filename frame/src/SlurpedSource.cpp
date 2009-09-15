#ident "file_id $Id: SlurpedSource.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SlurpedSource.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 14 20:00:54 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SlurpedSource.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SlurpedSource.h"
#include "SlurpedSourcePriv.h"
#include "Slurp.h"
#include "Parse.h"
#include "fwrap.h"

SlurpedSource::SlurpedSource()
{
    d_=new SlurpedSourcePriv;
}

SlurpedSource::~SlurpedSource( )
{
    delete d_;
}

void 
SlurpedSource::init(const char *filename )
{
    d_->cp_= Slurp::slurpFile(filename);
    //d_->pv_ = Parse::parse_rfc822_ptr( cp.get());
    do_parse( d_->cp_.get());
    d_->ve_=d_->pv_->end();
    d_->line_=-1;
};

bool 
SlurpedSource::next()
{
    if( d_->line_==-1 )
    {
        d_->vi_=d_->pv_->begin();
        d_->line_=0;
        return true;
    }
    ++d_->vi_;
    if( d_->vi_==d_->ve_ )
    {
        return false;
    }
    ++d_->line_;
    return true;
}

int
SlurpedSource::lineNum()
{
    return d_->line_;
}

char_ptr    
SlurpedSource::getCharPtr()
{
    char *s = *(d_->vi_);
    return ptr_strdup( s );
}

