#ident "file_id $Id: PreSortedProjection.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PreSortedProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Mon Mar 10 02:35:22 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: PreSortedProjection.cpp,v 1.6 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>

#include "PreSortedProjection.h"
#include "LiteralExpression.h"
#include "ptr_strdup.h"
#include "BaseException.h"
#include "dbg.h"

// ----------------------------------------------------------------------
// each call to next 
// 
// ----------------------------------------------------------------------
class PreSortedProjectionPriv
{
    private:
    friend class PreSortedProjection ;
    DelimitedSourcePtr  src_;
    ExpressionList      projection_;
    MetricList          metrics_;

    ExpressionList  compare_; //the values against which we check for sameness 
                              // also the projection expressions we update

    bool            firstrow_ ;
    bool            lastrow_ ;
    int             line_ ;
    PreSortedProjectionPriv( 
        DelimitedSourcePtr  &src,
        ExpressionList      &projection,
        MetricList          &metrics
    )
    : src_(src), projection_(projection), metrics_(metrics), 
    firstrow_(true), lastrow_(false), line_(0)
    {
        compare_=new expression_list_t ;
    };
};

PreSortedProjection::PreSortedProjection(
    DelimitedSourcePtr  &src,
    RelationMetaPtr     &meta,
    ExpressionList      &projection,
    MetricList          &metrics
)
: ProxyRelation( meta)
{
    d_=new PreSortedProjectionPriv( src, projection, metrics );
    if( d_->metrics_->size() + d_->projection_->size() != getNumColumns())
    {
        PANIC("metadata column count must match group by + metrics");
    }
}

PreSortedProjection::~PreSortedProjection()
{
    delete d_;
}

void 
PreSortedProjection::shuffle()
{
    clearExpressions();
    ExpressionListItr i=d_->compare_->begin();
    ExpressionListItr e=d_->compare_->end();
    for( ; i!=e; ++i )
    {
        addExpression( *i );
    }
    d_->compare_->clear();

    copy_projection_compare();
    
    MetricListItr im=d_->metrics_->begin();
    MetricListItr em=d_->metrics_->end();
    for( ; im!=em; ++im )
    {
        ExpressionPtr ep_metric( *im );
        addExpression( ep_metric );
        *im=(*im)->copy();
    }
}

void 
PreSortedProjection::copy_projection_compare()
{
    ExpressionListItr ip=d_->projection_->begin();
    ExpressionListItr ep=d_->projection_->end();
    for( ; ip!=ep; ++ip )
    {
        ExpressionPtr lep( new LiteralExpression( (*ip)->getRaw()));
        d_->compare_->push_back( lep );
    }
}

void 
PreSortedProjection::load()
{
    MetricListItr im=d_->metrics_->begin();
    MetricListItr em=d_->metrics_->end();
    for( ; im!=em; ++im )
    {
        (*im)->notify();
    }
}

bool 
PreSortedProjection::compare()
{
    //return true if this row and prev row are the same
    ExpressionListItr ip=d_->projection_->begin();
    ExpressionListItr ep=d_->projection_->end();
    ExpressionListItr ic=d_->compare_->begin();
    for( ; ip!=ep; ++ip, ++ic )
    {
        if( 0!= strcmp((*ip)->getRaw(), (*ic)->getRaw()))
        {
            return false;
        }
    }
    return true ;
}


int
PreSortedProjection::lineNum()
{
    return d_->line_;
}

bool
PreSortedProjection::childNotify()
{
    // ------------------------------------------------------------
    // the underlying stream is assumed to be ordered according to
    // the projection expressions such that were we to print out a 
    // string for every row composed of the projection expressions 
    // we would never find two instances of the same string separated
    // by a different string. 
    //
    // the algorithm here assumes that whenever this function is called
    // it has a brand new row loaded into thisrow_.
    // we pull rows into thisrow_ with load()
    // ------------------------------------------------------------
    // ------------------------------------------------------------ 
    //fprintf( stderr, "PreSortedProjection::childNotify() top\n" );
    //FDBG_SI( "PreSortedProjection::childNotify() top", this );
    ExpressionListItr ip=d_->projection_->begin();
    ExpressionListItr ep=d_->projection_->end();
    for( ; ip!=ep; ++ip )
    {
        (*ip)->notify();
    }
    if( d_->firstrow_==true )
    {

        load();
        copy_projection_compare();
        d_->firstrow_=false ;
        FDBG_SI( "PreSortedProjection::childNotify() returning false, first row", this );
        return false;
    }
    if( ! compare())//new row
    {
        shuffle();
        load();
        ++d_->line_;
        FDBG_SI( "PreSortedProjection::childNotify() returning true, new group", this );
        return true;
    }
    load();//same group by partition
    FDBG_SI( "PreSortedProjection::childNotify() returning false, same group", this );
    return false;
}

bool
PreSortedProjection::childNotifyLast()
{
    FDBG_SI( "PreSortedProjection::childNotifyLast() top", this );
    shuffle();
    ++d_->line_;
    n_->notify();
    FDBG_SI( "PreSortedProjection::childNotifyLast() returning true", this );
    return true;
}

