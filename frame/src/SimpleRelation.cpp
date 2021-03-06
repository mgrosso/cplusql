#ident "file_id $Id: SimpleRelation.cpp,v 1.11 2005/10/19 08:41:19 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SimpleRelation.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Nov 10 00:11:55 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SimpleRelation.cpp,v 1.11 2005/10/19 08:41:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SimpleRelation.h"
#include "ProxyRelationPriv.h"
#include "ColumnExpression.h"
#include "BaseException.h"
#define DEBUG
#include "dbg.h"

class SimpleRelationPriv
{
    private:
    friend class SimpleRelation ;
    DelimitedSourcePtr          srcp_;
    AbstractDelimitedSource *   src_;
    ExpressionList              filters_;
    ExpressionList              derived_;
    bool                        dont_filter_ ;
};

SimpleRelation::SimpleRelation(
    DelimitedSourcePtr &src,
    RelationMetaPtr &meta,
    ExpressionList &derivedColumns,
    ExpressionList &filters,
    bool only_derived
)
:ProxyRelation( meta )
{
   // fprintf(stderr, "SimpleRelation ctor this %x, only_derived %u, src %x, src numcol %x, derived numcol %u, meta numcol %u\n", 
   //     (unsigned )this, 
   //     (unsigned)only_derived, 
   //     src.get() ,
   //     src->getNumColumns(),
   //     derivedColumns->size() ,
   //     meta->getNumColumns()
   // );

    if(
        ( only_derived && derivedColumns->size() != meta->getNumColumns()) ||
        ( !only_derived && src->getNumColumns() + derivedColumns->size() != meta->getNumColumns())
    )
    {
        throw BaseException::factory( 
        "SimpleRelation::ctor(), invalid numColumns src[%i] derived[%i] meta[%i]",
            src->getNumColumns(), derivedColumns->size(), meta->getNumColumns()
        );
    }
    d_= new SimpleRelationPriv();
    d_->srcp_=src;
    d_->src_=d_->srcp_.get();
    d_->filters_=filters;
    d_->derived_=derivedColumns;
    d_->dont_filter_ = d_->filters_->size() > 0 ? false : true ;
    if( !only_derived )
    {
        for( size_t i=0; i< d_->src_->getNumColumns(); ++i )
        {
            ExpressionPtr e ( (Expression *)new ColumnExpression( d_->srcp_, i ));
            addExpression( e );
        }
    }
    ExpressionListItr b;
    ExpressionListItr e;
    for( b=derivedColumns->begin(), e=derivedColumns->end(); b!=e ; ++b )
    {
        addExpression(*b);
        //dont register here, we call notify() on these manually
        //during childNotify() so that they can be part of the where
        //clause.  
        //
        //registerListener( (*b).get());
    }
}

SimpleRelation::~SimpleRelation()
{
    //fprintf(stderr, "SimpleRelation top %x\n", (unsigned )this);
    delete d_;
    //fprintf(stderr, "SimpleRelation bottom %x\n", (unsigned )this);
}

bool
SimpleRelation::childNotify()
{
    //
    //it is important that we notify() derived columns first, since we
    //will want to be able to use them in where clauses.
    //
    ExpressionListItr b;
    ExpressionListItr e;
    //FDBG_SI( "SimpleRelation::childNotify() top ", this );
    for( 
        b=d_->derived_->begin(), e=d_->derived_->end(); 
        b!=e ; 
        ++b 
    )
    {
        ////FDBG_SII( "SimpleRelation::childNotify() top loop derived", this, (*b).get());
        (*b)->notify();
        //FDBG_SII( "SimpleRelation::childNotify() bot loop derived", this, (*b).get());
    }
    for( 
        b=d_->filters_->begin(), e=d_->filters_->end(); 
        b!=e ; 
        ++b 
    )
    {
        //FDBG_SII( "SimpleRelation::childNotify() top loop", this, (*b).get());
        (*b)->notify();
        if( ! (*b)->getBool() )
        {
            return false;
        }
        //FDBG_SII( "SimpleRelation::childNotify() loop", this, (*b).get());
    }
    return true;
}

bool
SimpleRelation::childNotifyLast()
{
    //
    //it is important that we notify() derived columns first, since we
    //will want to be able to use them in where clauses.
    //
    ExpressionListItr b;
    ExpressionListItr e;
    //FDBG_SI( "SimpleRelation::childNotify() top ", this );
    for( 
        b=d_->derived_->begin(), e=d_->derived_->end(); 
        b!=e ; 
        ++b 
    )
    {
        //FDBG_SII( "SimpleRelation::childNotify() top loop derived", this, (*b).get());
        (*b)->notifyLast();
        //FDBG_SII( "SimpleRelation::childNotify() bot loop derived", this, (*b).get());
    }
    return true;
}

int
SimpleRelation::lineNum()
{
    return d_->src_->lineNum();
}

