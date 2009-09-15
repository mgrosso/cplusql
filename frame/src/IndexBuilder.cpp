#ident "file_id $Id: IndexBuilder.cpp,v 1.1 2004/09/21 15:47:42 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IndexBuilder.cpp
// Author:    mgrosso 
// Created:   Tue Aug 17 15:22:57 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: IndexBuilder.cpp,v 1.1 2004/09/21 15:47:42 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "IndexBuilder.h"
#include "AppLog.h"

class IndexBuilderPriv 
{
    private:
    friend class IndexBuilder ;

    RelationPtr                 a_ ; 
    IndexPtr                    i_ ;
    ExpressionPtr               key_ ;
    ExpressionPtr               where_ ;
    IndexBuilderPriv( 
        RelationPtr &a, IndexPtr &i, 
        ExpressionPtr &key, ExpressionPtr &where);
};

IndexBuilderPriv::IndexBuilderPriv( 
    RelationPtr &a, IndexPtr &i, 
    ExpressionPtr &key, ExpressionPtr &where)
:a_(a),i_(i),key_(key),where_(where)
{
    //APPLOG_DBG("this %x", reinterpret_cast< unsigned int>(this));
}

IndexBuilder::IndexBuilder( 
    RelationPtr &a, IndexPtr &i, 
    ExpressionPtr &key, ExpressionPtr &where )
{
    d_=new IndexBuilderPriv ( a, i, key, where );
    d_->a_->registerListener( this );
    //APPLOG_DBG("this %x", reinterpret_cast< unsigned int>(this));
}

IndexBuilder::~IndexBuilder()
{
    delete d_;
}

void
IndexBuilder::notify()
{
    //APPLOG_DBG("this %x", reinterpret_cast< unsigned int>(this));
    if( ! d_->where_.get() || d_->where_->getBool())
    {
        d_->i_->add( d_->key_->getRaw(),d_->a_->lineNum());
    }
}

