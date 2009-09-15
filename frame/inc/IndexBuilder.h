
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IndexBuilder.h
// Author:    mgrosso 
// Created:   Tue Aug 17 15:22:57 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: IndexBuilder.h,v 1.1 2004/09/21 15:47:42 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef INDEXBUILDER_H
#define INDEXBUILDER_H 1

#include "Index.h"
#include "Listener.h"
#include "AbstractRelation.h"
#include "Expression.h"

class IndexBuilderPriv ;
class IndexBuilder : public Listener
{
    public:
    IndexBuilder( RelationPtr &a, IndexPtr &i, 
        ExpressionPtr &key, ExpressionPtr &where );
    virtual ~IndexBuilder();

    void notify();

    private:
    IndexBuilderPriv *d_;
    //not implemented
    IndexBuilder();
    IndexBuilder( const IndexBuilder &rhs );
    IndexBuilder &operator=( const IndexBuilder &rhs );
};



#endif /* INDEXBUILDER_H */
