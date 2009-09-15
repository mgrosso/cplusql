// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NumericProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: NumericProjection.h,v 1.2 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef NUMERICPROJECTION_H
#define NUMERICPROJECTION_H 1

#include "AbstractProjection.h"

class NumericProjectionPriv ;

class NumericProjection : public AbstractProjection
{
    protected:
    NumericProjection();

    public:
    virtual ~NumericProjection();
    virtual void                        notify();
    virtual void                        childNotify()=0;
    virtual ptr< AbstractProjection >    copy()=0;

    //from Expression
    virtual bool                isNull()const ;
    virtual const char *        getRaw();
    virtual char_ptr            getCharPtr()const ;
    virtual bool                getBool()const ;
    virtual long int            getLong()const ;
    virtual double              getDouble()const ;
    virtual long long           getLongLong()const ;
    virtual long double         getLongDouble()const ;
    protected:
    long double                      result_ ;
    char_ptr                    resultc_ ;


    private:
    //helpers
    void                        makeString();
    static const size_t         buflen ;
    static const char * const   dummybytes ;
    
    //data
    NumericProjectionPriv  *    d_;

    //not implemented
    NumericProjection(const NumericProjection &rhs);
    NumericProjection &operator=(const NumericProjection &rhs);
};

#endif /* NUMERICPROJECTION_H */
