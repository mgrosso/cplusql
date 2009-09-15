// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Line:      SingleRowDelimitedSource.h
// Author:    djain
// Created:   Sun Aug 11 20:42:55 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SingleRowDelimitedSource.h,v 1.1 2004/01/16 21:01:33 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SINGLEDELIMITEDSOURCE_H
#define SINGLEDELIMITEDSOURCE_H 1

#include "AbstractDelimitedSource.h"
#include "AbstractDelimitedSourceNext.h"

class SingleRowDelimitedSourcePriv ;

class SingleRowDelimitedSource : public AbstractDelimitedSource
{
    public:
    SingleRowDelimitedSource( DelimitedSourcePtr & ds );

    virtual                     ~SingleRowDelimitedSource();
    virtual const char *        getRaw( size_t index ) ;
    virtual const char *        getRaw( size_t index ) const ;
    virtual char_ptr            getCharPtr( size_t index ) const ;
    virtual bool                getBool( size_t index ) const ;
    virtual long                getLong( size_t index ) const ;
    virtual double              getDouble( size_t index ) const ;
    virtual long long           getLongLong( size_t index ) const ;
    virtual long double         getLongDouble( size_t index ) const ;
	virtual int					lineNum();
	virtual size_t				getNumColumns() const;
	

    private:
    SingleRowDelimitedSourcePriv *d_ ;

    //not implemented
    SingleRowDelimitedSource();
    SingleRowDelimitedSource( const SingleRowDelimitedSource &rhs);
    SingleRowDelimitedSource &operator=( const SingleRowDelimitedSource &rhs);
};

#endif /* SINGLEDELIMITEDSOURCE_H */
