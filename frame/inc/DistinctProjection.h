// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DistinctProjection.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Dec 15 17:08:36 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DistinctProjection.h,v 1.2 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DISTINCTPROJECTION_H
#define DISTINCTPROJECTION_H 1

#include "AbstractProjection.h"
#include "map.h"

typedef map< char_ptr, AbstractProjectionPtr, char_ptr_less_than >
    distinct_map_t ;

class DistinctProjection : public AbstractProjection
{
    public:
    DistinctProjection( 
        ExpressionPtr &groupby_col, 
        AbstractProjectionPtr &next 
    );
    virtual                                 ~DistinctProjection();
    virtual void                            notify();
    virtual ptr< AbstractProjection >       copy()=0;

    //from Expression
    virtual bool                isNull()const=0 ;
    virtual const char *        getRaw()const=0 ;
    virtual char_ptr            getCharPtr()const=0 ;
    virtual bool                getBool()const=0 ;
    virtual long int            getLong()const=0 ;
    virtual double              getDouble()const=0 ;
    virtual long long           getLongLong()const=0 ;

    private:
    AbstractProjectionPtr       nextp_ ;
    ExpressionPtr               groupby_col ;
    distinct_map_t              distinct_ ;
    distinct_map_t::iterator    current_row_;
    bool                        started_ ;


    //not implemented
    DistinctProjection(const DistinctProjection &rhs);
    DistinctProjection &operator=(const DistinctProjection &rhs);
};

#endif /* DISTINCTPROJECTION_H */
