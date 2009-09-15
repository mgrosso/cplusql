// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: StringProjection.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef STRINGPROJECTION_H
#define STRINGPROJECTION_H 1

#include "AbstractProjection.h"

class StringProjectionPriv ;

class StringProjection : public AbstractProjection
{
    public:
    virtual ~StringProjection();
    virtual void                        notify();
    virtual void                        childNotify()=0;
    virtual ptr< AbstractProjection >    copy()=0;

    //from Expression
    virtual bool                isNull()const ;
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual char_ptr            getCharPtr()const ;
    virtual bool                getBool()const ;
    virtual long int            getLong()const ;
    virtual double              getDouble()const ;
    virtual long long           getLongLong()const ;
    virtual long double         getLongDouble()const ;

    protected:
    StringProjection();
    void set( char_ptr &s );
    void set( char_ptr s );

    private:
    StringProjectionPriv *d_;
    
    StringProjection(const StringProjection &rhs);
    StringProjection &operator=(const StringProjection &rhs);
};



#endif /* STRINGPROJECTION_H */
