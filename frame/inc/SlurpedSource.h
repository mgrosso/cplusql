// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SlurpedSource.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 14 20:01:00 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SlurpedSource.h,v 1.1 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SLURPEDSOURCE_H
#define SLURPEDSOURCE_H 1

#include <stdio.h>
#include "ptr.h"
#include "ptr_strdup.h"
#include "Source.h"

class SlurpedSourcePriv ;

class SlurpedSource : public Source
{
    public:
    virtual             ~SlurpedSource();
    virtual bool        next();
    virtual int         lineNum() ;
    virtual char_ptr    getCharPtr() ;

    protected:
    SlurpedSource();
    SlurpedSourcePriv *d_ ;
    virtual void do_parse( char * )=0;
    void init(const char *filename);

    private:

    //not implemented
    SlurpedSource( const SlurpedSource &rhs);
    SlurpedSource &operator=( const SlurpedSource &rhs);
};

typedef ptr< SlurpedSource > SlurpedSourcePtr ;


#endif /* SLURPEDSOURCE_H */
