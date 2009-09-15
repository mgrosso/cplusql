#ident "file_id $Id: StringArrayRelation.cpp,v 1.3 2004/09/21 23:55:16 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArrayRelation.cpp
// Author:    mgrosso 
// Created:   Thu Jul  8 01:44:38 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArrayRelation.cpp,v 1.3 2004/09/21 23:55:16 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "frame_config.h"
#include <vector>
#include "StringArrayRelation.h"
#include "CharExpression.h"
#include "BaseException.h"
#include "AppLog.h"

using namespace std;

class StringArrayRelationPriv 
{
    private:
    friend class StringArrayRelation ;

    StringArrayPtr                      sa_ ;
    size_t                              row_ ;
    vector< CharExpression * >          ve_ ;
    size_t                              columns_ ;
    size_t                              maxrow_ ;

    StringArrayRelationPriv( StringArrayPtr &sa, size_t cols );
};

StringArrayRelationPriv::StringArrayRelationPriv( StringArrayPtr &sa, size_t cols )
:sa_(sa), row_(0), columns_( cols ), maxrow_(sa->size()/cols)
{
    --row_;//point to -1, so first notify() will set to zero.
}

StringArrayRelation::StringArrayRelation( RelationMetaPtr &meta, StringArrayPtr &sa)
:ProxyRelation( meta )
{
    if(!meta->getNumColumns())
    {
        PANICV("need at least one column or else it isnt a relation, is it?");
    }
    d_=new StringArrayRelationPriv ( sa, meta->getNumColumns());
    for( size_t i = 0; i < d_->columns_; ++i )
    {
        CharExpression *ce = new CharExpression( NULL );
        ExpressionPtr e ( ce );
        addExpression( e );
        d_->ve_.push_back( ce );
    }
}

StringArrayRelation::~StringArrayRelation()
{
    delete d_;
}

bool
StringArrayRelation::childNotify()
{
    //APPLOG_DBG(".");
    size_t e=d_->columns_;
    size_t offset = d_->row_ * e ;
    for( size_t i=0 ; i< e; ++i )
    {
        (d_->ve_[i])->set( d_->sa_->get( offset + i ));
    }
    return true;
}

int
StringArrayRelation::lineNum()
{
    return static_cast<int>(d_->row_);
}

void
StringArrayRelation::run()
{
    d_->maxrow_ = d_->sa_->size() / d_->columns_ ;
    //APPLOG_DBG("top %lu, %lu, %lu", d_->maxrow_, d_->columns_, d_->sa_->size() );
    for( d_->row_=0; d_->row_ < d_->maxrow_; ++d_->row_ )
    {
        //APPLOG_DBG("notify.");
        notify();
    }
    //APPLOG_DBG("last");
    notifyLast();
}

