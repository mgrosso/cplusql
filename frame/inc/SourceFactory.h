// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SourceFactory.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 28 00:47:23 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SourceFactory.h,v 1.2 2003/06/10 01:54:42 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SOURCEFACTORY_H
#define SOURCEFACTORY_H 1

#include "Source.h"
#include "DelimitedSource.h"
#include "ptr_strdup.h"


class SourceFactory
{
    public:
    static SourcePtr getRawSource( const char *filename );
    static SourcePtr getLineSource( const char *filename );
    static SourcePtr getRFC822Source( const char *filename );
    static DelimitedSourcePtr getDelimitedSource( 
        const char *filename, const char *sepchars, size_t fields 
    );

    static SourcePtr getRawMultiSource( const char *filename );
    static SourcePtr getLineMultiSource( const char *filename );
    static SourcePtr getRFC822MultiSource( const char *filename );
    static DelimitedSourcePtr getDelimitedMultiSource(
        const char *filename_pattern, const char *sepchars, size_t fields 
    );
    static DelimitedSourcePtr getDelimitedMultiSources(
        list< char_ptr> &patterns, const char *sepchars, size_t fields 
    );
    static DelimitedSourcePtr getNoCopyDelimitedMultiSources(
        list< char_ptr> &patterns, char sepchar, size_t fields , size_t bufsize
    );

    private:
    static SourcePtr getMultiSource( 
        const char *filename_pattern, 
        SourcePtr(*factory)(const char *)
    );
    static SourcePtr getMultiSources( 
        list< char_ptr > &patterns,
        SourcePtr(*factory)(const char *)
    );
    static DelimitedSourcePtr giveback( DelimitedSource *s );
    static SourcePtr giveback( Source *s );
};


#endif /* SOURCEFACTORY_H */
