#ident "file_id $Id: StringArrayOrderedRelation.cpp,v 1.2 2004/12/31 21:50:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArrayOrderedRelation.cpp
// Author:    mgrosso 
// Created:   Sun Aug 15 21:20:29 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArrayOrderedRelation.cpp,v 1.2 2004/12/31 21:50:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StringArrayOrderedRelation.h"
#include "CharExpression.h"
#include "BaseException.h"
#include "AppLog.h"
#include <vector>

using namespace std;

class StringArrayOrderedRelationPriv 
{
    private:
    friend class StringArrayOrderedRelation ;
    StringArrayPtr                      sa_ ;
    size_t                              row_ ;
    size_t                              columns_ ;
    vector< CharExpression * >          ve_ ;
    Index::Iterator                bottom_ ;
    Index::Iterator                current_ ;
    Index::Iterator                top_ ;
    void    reset( 
        Index::Iterator bottom_inclusive, Index::Iterator top_exclusive);
    StringArrayOrderedRelationPriv(
        StringArrayPtr &sa, size_t columns,
        Index::Iterator bottom_inclusive, Index::Iterator top_exclusive);
};

StringArrayOrderedRelationPriv::StringArrayOrderedRelationPriv(
    StringArrayPtr &sa, size_t columns, 
        Index::Iterator bottom_inclusive, Index::Iterator top_exclusive  )
:sa_(sa),row_(static_cast<unsigned int>(-1)),columns_(columns),
bottom_(bottom_inclusive),current_(bottom_inclusive),top_(top_exclusive)
{
    if(!columns)
    {
        PANICV("need at least one column or else it isnt a relation, is it?");
    }
}

void
StringArrayOrderedRelation::initv()
{
    for( size_t i = 0; i < d_->columns_; ++i )
    {
        CharExpression *ce = new CharExpression( NULL );
        ExpressionPtr e ( ce );
        addExpression( e );
        d_->ve_.push_back( ce );
    }
}

StringArrayOrderedRelation::StringArrayOrderedRelation(
    RelationMetaPtr &meta, StringArrayPtr &sa, 
        Index::Iterator bottom_inclusive, Index::Iterator top_exclusive  )
:ProxyRelation( meta )
{
    d_ = new StringArrayOrderedRelationPriv ( sa, getNumColumns(), 
        bottom_inclusive, top_exclusive );
    initv();
}

StringArrayOrderedRelation::StringArrayOrderedRelation(
    RelationMetaPtr &meta, StringArrayPtr &sa, IndexPtr &i
) :ProxyRelation( meta )
{
    d_=new StringArrayOrderedRelationPriv ( sa, getNumColumns(), 
        i->begin(), i->end() );
    initv();
}

StringArrayOrderedRelation::~StringArrayOrderedRelation()
{
    delete d_;
}

void
StringArrayOrderedRelationPriv::reset( 
    Index::Iterator bottom_inclusive, Index::Iterator top_exclusive  )
{
    bottom_= bottom_inclusive;
    top_   = top_exclusive;
    current_   = bottom_ ;
    row_ = static_cast<size_t>(-1) ;
}

void
StringArrayOrderedRelation::reset( 
    Index::Iterator bottom_inclusive, Index::Iterator top_exclusive  )
{
    d_->reset( bottom_inclusive, top_exclusive );
}

int
StringArrayOrderedRelation::lineNum() 
{
    return static_cast<int>(d_->row_);
}

bool
StringArrayOrderedRelation::childNotify()
{
    size_t e=d_->columns_;
    size_t offset = (*d_->current_) * e ;
    //APPLOG_DBG(
    //    "row_=%lu, *d_->current_=%lu d_->current_.getKey()=%s, offset=%lu e=%lu", 
    //    d_->row_+1, *d_->current_, d_->current_.getKey(), offset, e );
    for( size_t i=0 ; i< e; ++i )
    {
        (d_->ve_[i])->set( d_->sa_->get( offset ));
        ++offset;
    }
    ++d_->row_ ;
    return true;
}

void 
StringArrayOrderedRelation::run()
{
    d_->current_ = d_->bottom_ ;
    while( ! (d_->current_ == d_->top_ ))
    {
        notify();
        ++d_->current_ ;
    }
    notifyLast();
}

