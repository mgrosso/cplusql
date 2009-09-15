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
// $Id: AbstractDelimitedSource.h,v 1.4 2004/09/20 20:50:34 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACT_DELIMITED_SOURCE_H
#define ABSTRACT_DELIMITED_SOURCE_H 1

//this is for  size_t
#include <sys/types.h>
#include "ptr_strdup.h"
#include "AbstractSimpleMeta.h"

class AbstractDelimitedSource : virtual public AbstractSimpleMeta
{
    protected:
                                AbstractDelimitedSource();
    public:
    virtual                     ~AbstractDelimitedSource();

    virtual int                 lineNum()=0 ;

    virtual const char *        getRaw( size_t index ) =0 ;
    virtual char_ptr            getCharPtr( size_t index ) const =0 ;
    virtual bool                getBool( size_t index ) const =0 ;
    virtual long                getLong( size_t index ) const =0 ;
    virtual long long           getLongLong( size_t index ) const =0 ;
    virtual double              getDouble( size_t index ) const =0 ;
    virtual long double         getLongDouble( size_t index ) const =0 ;

    private:
    //not implemented
    AbstractDelimitedSource( const AbstractDelimitedSource &rhs);
    AbstractDelimitedSource &operator=( const AbstractDelimitedSource &rhs);
};

typedef ptr< AbstractDelimitedSource > DelimitedSourcePtr ;

#endif /* ABSTRACT_DELIMITED_SOURCE_H */
