// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DoubleProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DoubleProjection.h,v 1.3 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DOUBLEPROJECTION_H
#define DOUBLEPROJECTION_H 1

#include "AbstractProjection.h"

class DoubleProjection : public AbstractProjection
{
    protected:
    DoubleProjection();
    long double                 result_;

    public:
    virtual ~DoubleProjection();
    virtual void                        notify();
    virtual AbstractProjectionPtr       copy()=0;

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
    virtual void                childNotify()=0;

    private:
    void                        makeString();
    char_ptr                    resultc_;
    static const size_t         buflen ;


    DoubleProjection(const DoubleProjection &rhs);
    DoubleProjection &operator=(const DoubleProjection &rhs);

    static const char *const dummybytes ;
};

#endif /* DOUBLEPROJECTION_H */
