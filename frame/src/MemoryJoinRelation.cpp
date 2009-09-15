#ident "file_id $Id: MemoryJoinRelation.cpp,v 1.3 2005/02/01 01:50:15 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MemoryJoinRelation.cpp
// Author:    mgrosso 
// Created:   Sun Aug 22 19:05:04 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: MemoryJoinRelation.cpp,v 1.3 2005/02/01 01:50:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MemoryJoinRelation.h"
#include <vector>
#include "CharExpression.h"
#include "ColumnExpression.h"
#include "AppLog.h"

using namespace std ;

class MemoryJoinRelationPriv 
{
    private:
    friend class MemoryJoinRelation ;
    vector< CharExpression * >          ve_ ;
    size_t                              row_ ;
    
    RelationPtr                         left_ ;
    StringArrayPtr                      right_ ;
    ExpressionPtr                       lkey_ ;
    IndexPtr                            rkey_ ;
    bool                                outer_ ;
    
    void                                fillLeft()
    {
        size_t offset = left_->getNumColumns();
        for( size_t i=0 ; i < offset; ++i )
        {
            (ve_[i])->set( left_->getRaw( i ));
        }
    };
    MemoryJoinRelationPriv(
        RelationPtr &left,
        StringArrayPtr &right,
        ExpressionPtr &lkey,
        IndexPtr &rkey,
        bool outer
        )
    :row_(0),left_(left),right_(right),lkey_(lkey),rkey_(rkey),
        outer_(outer)
    {
        ;//noop
    };
};


MemoryJoinRelation *
MemoryJoinRelation::make(
        RelationPtr &left,
        StringArrayPtr &right,
        RelationMetaPtr &right_meta,
        ExpressionPtr &left_key,
        IndexPtr &right_key_lookup,
        bool outer,
        const char *my_name
        )
{
    RelationMetaPtr rm( new RelationMeta());
    rm->setRelationName( my_name );
    size_t lcols = left->getNumColumns();
    size_t rcols = right_meta->getNumColumns();
    size_t i;
    string s;
    for( i=0; i< lcols; ++i )
    {
        s = left->getRelationName();
        s += "_" ;
        s += left->getColumnName( i );
        rm->addColumn( s.c_str());
    }
    for( i=0; i< rcols; ++i )
    {
        s = right_meta->getRelationName();
        s += "_" ;
        s += right_meta->getColumnName( i );
        rm->addColumn( s.c_str());
    }
    return new MemoryJoinRelation( 
        rm, left, right, left_key, right_key_lookup, outer );
}

MemoryJoinRelation::MemoryJoinRelation(
        RelationMetaPtr &meta,
        RelationPtr &left,
        StringArrayPtr &right,
        ExpressionPtr &lkey,
        IndexPtr &rkey,
        bool outer
)
:ProxyRelation( meta )
{
    d_=new MemoryJoinRelationPriv (left, right, lkey, rkey, outer);
    size_t i;
    size_t leftcol = d_->left_->getNumColumns(); 
    size_t rightcol = getNumColumns() - leftcol ;

    for( i = 0; i < leftcol ; ++i )
    {
        ExpressionPtr e(new ColumnExpression( d_->left_, i ));
        addExpression(e);
    }
    for( i = 0; i < rightcol; ++i )
    {
        CharExpression *chare = new CharExpression( NULL );
        ExpressionPtr e( chare );
        addExpression(e);
        d_->ve_.push_back( chare );
    }
    left->registerListener(this);
}

MemoryJoinRelation::~MemoryJoinRelation()
{
    delete d_;
}

int
MemoryJoinRelation::lineNum()
{
    return d_->row_ ;
}

bool
MemoryJoinRelation::childNotify()
{
    //this pure virtual function must be implemented, but it will never
    //be called because instead we override notify().
    APPLOG_WARN( "hmm, this method should never get called."); 
    return true ;
}

void
MemoryJoinRelation::notify()
{
    Index::bounds_type b = d_->rkey_->bounds( d_->lkey_->getRaw());
    size_t rightcols = getNumColumns() - d_->left_->getNumColumns();
    size_t i;

    //APPLOG_DBG( "lkey %s top", d_->lkey_->getRaw());
    //if( b.first != d_->rkey_->end())
    //{
    //    APPLOG_DBG( "b.first=%s", b.first.getKey());
    //}
    //if( b.second != d_->rkey_->end())
    //{
    //    APPLOG_DBG( "b.second=%s", b.second.getKey());
    //}

    //if we have no matching rows from the right side of the relation

    if( b.first == d_->rkey_->end() ||
        b.first == b.second || 
        0 != strcmp(d_->lkey_->getRaw(), b.first.getKey()))
    {
        if( ! d_->outer_ )
        {
            //APPLOG_DBG( "lkey %s, no match, ! outer", d_->lkey_->getRaw());
            return;
        }
        //d_->fillLeft();
        //APPLOG_DBG( "lkey %s, no match, outer true", d_->lkey_->getRaw());
        for( i=0 ; i < rightcols; ++i )
        {
            (d_->ve_[i])->set( "" );
        }
        ++d_->row_;
        n_->notify();
        return;
    }
    //APPLOG_DBG( "lkey %s, match top", d_->lkey_->getRaw());
    //d_->fillLeft();
    for( ; b.first != b.second; ++b.first )
    {
        //hack to work around broken Index::Iterator ==
        if(0!=strcmp(d_->lkey_->getRaw(), b.first.getKey()))
        {
            break;
        }
        //APPLOG_DBG( "lkey %s, matches %lu, %s", 
         //d_->lkey_->getRaw(), *b.first, b.first.getKey());
        for( i=0 ; i < rightcols; ++i )
        {
            (d_->ve_[i])->set( d_->right_->get( *b.first * rightcols + i ));
        }
        ++d_->row_;
        n_->notify();
    }
    //APPLOG_DBG( "lkey %s, matches bottom", d_->lkey_->getRaw());
}

