// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Dec 13 02:21:29 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractProjection.h,v 1.4 2003/10/28 22:22:40 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACTPROJECTION_H
#define ABSTRACTPROJECTION_H 1

#include "Expression.h"
#include "ptr.h"
#include <vector>

class AbstractProjection : public Expression
{
    protected:
    AbstractProjection();
    public:
    virtual ~AbstractProjection();
    virtual void                        notify()=0;
    virtual ptr< AbstractProjection >    copy()=0;

    //from Expression
    virtual bool                isNull()const=0 ;
    virtual const char *        getRaw()=0 ;
    virtual char_ptr            getCharPtr()const=0 ;
    virtual bool                getBool()const=0 ;
    virtual long int            getLong()const=0 ;
    virtual double              getDouble()const=0 ;
    virtual long long           getLongLong()const=0 ;
    virtual long double         getLongDouble()const=0 ;

    private:
    AbstractProjection(const AbstractProjection &rhs);
    AbstractProjection &operator=(const AbstractProjection &rhs);
};

typedef ptr< AbstractProjection >                   AbstractProjectionPtr ;
typedef vector< AbstractProjectionPtr >             metric_list_t ;
typedef ptr< vector< AbstractProjectionPtr > >      MetricList ;
typedef vector< AbstractProjectionPtr >::iterator   MetricListItr ;


#endif /* ABSTRACTPROJECTION_H */
