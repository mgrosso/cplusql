
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DelayStubDelimitedSource.h
// Author:    mgrosso 
// Created:   Sun Jan  2 21:47:18 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: DelayStubDelimitedSource.h,v 1.2 2005/01/05 03:01:51 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DELAYSTUBDELIMITEDSOURCE_H
#define DELAYSTUBDELIMITEDSOURCE_H 1
#include "StubDelimitedSource.h"

class DelayStubDelimitedSourcePriv ;
class DelayStubDelimitedSource : public StubDelimitedSource
{
    public:
    DelayStubDelimitedSource( time_t seconds, long nanosecs );
    virtual ~DelayStubDelimitedSource();
    virtual int               lineNum();
    virtual bool              next();

    private:
    DelayStubDelimitedSourcePriv *d_;
    //not implemented
    DelayStubDelimitedSource();
    DelayStubDelimitedSource( const DelayStubDelimitedSource &rhs );
    DelayStubDelimitedSource &operator=( const DelayStubDelimitedSource &rhs );
};

#endif /* DELAYSTUBDELIMITEDSOURCE_H */

