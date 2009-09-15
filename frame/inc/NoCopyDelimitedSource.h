// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Line:      NoCopyDelimitedSource.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Aug 11 20:42:55 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: NoCopyDelimitedSource.h,v 1.5 2004/12/31 21:06:39 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef NOCOPYLINESOURCE_H
#define NOCOPYLINESOURCE_H 1

#include <stdio.h>
#include "ptr.h"
#include "ptr_strdup.h"
#include "ByteSource.h"
#include "AbstractDelimitedSourceNext.h"

class NoCopyDelimitedSourcePriv ;

class NoCopyDelimitedSource : public AbstractDelimitedSourceNext
{
    public:
    NoCopyDelimitedSource( ByteSourcePtr &bsp, size_t buflen, size_t fields, char delim, size_t badparse_maxrows = 0  );

    virtual                     ~NoCopyDelimitedSource();
    virtual bool                next();
    virtual int                 lineNum() ;
    virtual size_t              getNumColumns() const ;
    virtual const char *        getRaw( size_t index ) ;
    const char *                getRaw( size_t index ) const ;
    virtual char_ptr            getCharPtr( size_t index ) const ;
    virtual bool                getBool( size_t index ) const ;
    virtual long                getLong( size_t index ) const ;
    virtual double              getDouble( size_t index ) const ;
    virtual long long           getLongLong( size_t index ) const ;
    virtual long double         getLongDouble( size_t index ) const ;

    private:
    NoCopyDelimitedSourcePriv *d_ ;

    //not implemented
    NoCopyDelimitedSource();
    NoCopyDelimitedSource( const NoCopyDelimitedSource &rhs);
    NoCopyDelimitedSource &operator=( const NoCopyDelimitedSource &rhs);
};

#endif /* NOCOPYLINESOURCE_H */
