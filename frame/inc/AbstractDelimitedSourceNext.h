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
// $Id: AbstractDelimitedSourceNext.h,v 1.3 2005/01/24 18:20:59 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACT_DELIMITED_SOURCE_NEXT_H
#define ABSTRACT_DELIMITED_SOURCE_NEXT_H 1

#include "AbstractDelimitedSource.h"

class AbstractDelimitedSourceNext : virtual public AbstractDelimitedSource
{
    protected:
                            AbstractDelimitedSourceNext();
    public:
    virtual                 ~AbstractDelimitedSourceNext();
    virtual bool            next()=0;

    private:
    //not implemented
    AbstractDelimitedSourceNext( const AbstractDelimitedSourceNext &rhs);
    AbstractDelimitedSourceNext &operator=( const AbstractDelimitedSourceNext &rhs);
};

typedef ptr< AbstractDelimitedSourceNext > AbstractDelimitedSourceNextPtr ;

#endif /* ABSTRACT_DELIMITED_SOURCE_NEXT_H */
