
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AvgProjection.h
// Author:    mgrosso 
// Created:   Sun Sep  5 14:22:24 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: AvgProjection.h,v 1.1 2004/09/20 23:26:06 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef AVGPROJECTION_H
#define AVGPROJECTION_H 1
#include "DoubleProjection.h"
    
class AvgProjectionPriv ;
class AvgProjection : public DoubleProjection
{
    public:
    AvgProjection( ExpressionPtr &src );
    virtual ~AvgProjection();
    virtual void                            childNotify();
    virtual ptr< AbstractProjection >       copy();

    private:
    AvgProjectionPriv *d_;
    //not implemented
    AvgProjection();
    AvgProjection( const AvgProjection &rhs );
    AvgProjection &operator=( const AvgProjection &rhs );
};



#endif /* AVGPROJECTION_H */
