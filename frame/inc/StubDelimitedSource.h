
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StubDelimitedSource.h
// Author:    mgrosso 
// Created:   Sun Jan  2 21:50:02 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: StubDelimitedSource.h,v 1.2 2005/01/05 03:01:52 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STUBDELIMITEDSOURCE_H
#define STUBDELIMITEDSOURCE_H 1

#include "AbstractDelimitedSourceNext.h"

class StubDelimitedSourcePriv ;
class StubDelimitedSource : public AbstractDelimitedSourceNext
{
    protected:
    StubDelimitedSource();
    public:
    virtual ~StubDelimitedSource();

    //Children must implement only the following two functions:
    //virtual int               lineNum()=0 ;
    //virtual bool              next()=0;

    //There are zero columns...
    virtual     size_t          getNumColumns()const ;

    //These are stubs. dont try to call them...
    virtual const char *        getRaw( size_t index ) ;
    virtual char_ptr            getCharPtr( size_t index ) const ;
    virtual bool                getBool( size_t index ) const ;
    virtual long                getLong( size_t index ) const ;
    virtual long long           getLongLong( size_t index ) const ;
    virtual double              getDouble( size_t index ) const ;
    virtual long double         getLongDouble( size_t index ) const ;

    private:
    //not implemented
    StubDelimitedSource( const StubDelimitedSource &rhs );
    StubDelimitedSource &operator=( const StubDelimitedSource &rhs );
};



#endif /* STUBDELIMITEDSOURCE_H */

