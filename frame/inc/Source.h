// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DelimitedSource.h
// Author:    mgrosso Matthew Grosso
// Created:   Tue Dec  5 17:30:23 EST 2000 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: Source.h,v 1.1 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SOURCE_H
#define SOURCE_H 1

#include "ptr.h"
#include "ptr_strdup.h"

class Source
{
    public:
    virtual             ~Source();
    virtual bool        next()=0;
    virtual char_ptr    getCharPtr()=0 ;
    virtual int         lineNum()=0 ;

    protected:
    Source();

    private:
    //not implemented
    Source( const Source &rhs);
    Source &operator=( const Source &rhs);
};

typedef ptr< Source > SourcePtr ;

#endif /* SOURCE_H */
