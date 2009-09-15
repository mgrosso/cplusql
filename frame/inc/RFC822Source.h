// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RFC822Source.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 15 13:15:40 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: RFC822Source.h,v 1.1 2003/01/22 05:45:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef RFC822SOURCE_H
#define RFC822SOURCE_H 1

#include "ptr.h"
#include "ptr_strdup.h"
#include "SlurpedSource.h"

class RFC822Source : public SlurpedSource
{
    public:
    RFC822Source( const char *filename );
    virtual             ~RFC822Source();

    protected:
    virtual void do_parse( char * );

    private:

    //not implemented
    RFC822Source();
    RFC822Source( const RFC822Source &rhs);
    RFC822Source &operator=( const RFC822Source &rhs);
};

typedef ptr< RFC822Source > RFC822SourcePtr ;

#endif /* FILESOURCE_H */

