// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractRelationMeta.h
// Author:    mgrosso 
// Created:   Sun Aug 15 18:32:58 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: AbstractRelationMeta.h,v 1.1 2004/09/20 20:30:36 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACTRELATIONMETA_H
#define ABSTRACTRELATIONMETA_H 1

#include "ptr_strdup.h"
#include "AbstractSimpleMeta.h"

class AbstractRelationMeta : virtual public AbstractSimpleMeta
{
    public:
    virtual                     ~AbstractRelationMeta();

    virtual     const char *    getRelationName()const =0 ;
    virtual     char_ptr        getRelationHeader()const =0 ;
    virtual     const char *    getColumnName( size_t index )const =0 ;
    virtual     size_t          getColumnIndex( const char *name )const =0 ;

    protected:
    AbstractRelationMeta();
    AbstractRelationMeta( const AbstractRelationMeta &rhs );
    AbstractRelationMeta &operator=( const AbstractRelationMeta &rhs );

    private:
    //not implemented
};
typedef ptr< AbstractRelationMeta > AbstractRelationMetaPtr ;

#endif /* ABSTRACTRELATIONMETA_H */
