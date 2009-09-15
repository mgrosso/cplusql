
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractSimpleMeta.h
// Author:    mgrosso 
// Created:   Sun Aug 15 20:02:17 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: AbstractSimpleMeta.h,v 1.1 2004/09/20 20:30:36 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ABSTRACTSIMPLEMETA_H
#define ABSTRACTSIMPLEMETA_H 1

//for size_t
#include "frame_config.h"

class AbstractSimpleMeta
{
    public:
    virtual                     ~AbstractSimpleMeta();
    virtual     size_t          getNumColumns()const =0 ;

    protected:
                                AbstractSimpleMeta();
    private:
    //not implemented
    AbstractSimpleMeta( const AbstractSimpleMeta &rhs );
    AbstractSimpleMeta &operator=( const AbstractSimpleMeta &rhs );
};



#endif /* ABSTRACTSIMPLEMETA_H */
