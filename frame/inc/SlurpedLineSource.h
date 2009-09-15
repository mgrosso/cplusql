// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SlurpedLineSource.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 14 20:01:00 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SlurpedLineSource.h,v 1.1 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SLURPEDLINESOURCE_H
#define SLURPEDLINESOURCE_H 1

#include "SlurpedSource.h"

class SlurpedLineSource : public SlurpedSource
{
    public:
    SlurpedLineSource( const char *filename );
    virtual             ~SlurpedLineSource();

    protected:
    virtual void do_parse( char * );

    private:

    //not implemented
    SlurpedLineSource();
    SlurpedLineSource( const SlurpedLineSource &rhs);
    SlurpedLineSource &operator=( const SlurpedLineSource &rhs);
};

typedef ptr< SlurpedLineSource > SlurpedLineSourcePtr ;


#endif /* SLURPEDLINESOURCE_H */
