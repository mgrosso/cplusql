#ident "file_id $Id: RelationMeta.cpp,v 1.8 2005/04/26 15:17:41 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RelationMeta.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Tue Nov  5 05:14:36 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: RelationMeta.cpp,v 1.8 2005/04/26 15:17:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include <map>
#include <vector>
#include <string.h>
#include "hashFuncs.h"
#include "RelationMeta.h"
#include "BaseException.h"
#include "ptr_strdup.h"
#include "AppLog.h"

typedef map< const char *, size_t, cc_less_than > charpmap_t;

class RelationMetaPriv 
{
private:
    friend class RelationMeta;
    vector< char_ptr > n2s_;
    charpmap_t s2n_;
    char_ptr relname_;
};

RelationMeta::RelationMeta()
{
    rm_= new RelationMetaPriv();
}

RelationMeta::~RelationMeta()
{
    delete rm_;
}

void           
RelationMeta::setRelationName( const char *relname )
{
    rm_->relname_=ptr_strdup_wnull(relname);
}

const char *   
RelationMeta::getRelationName() const
{
    return rm_->relname_.get();
}

char_ptr
RelationMeta::getRelationHeader() const
{
    return ptr_strdup_ptr_vec( rm_->n2s_, "|", false );
}

size_t
RelationMeta::getNumColumns() const
{
    return rm_->n2s_.size();
}

const char *   
RelationMeta::getColumnName( size_t index ) const
{
    if( index >= rm_->n2s_.size())
    {
        PANIC_I( "RelationMeta::getColumnName(), out of bounds", index );
    }
    APPLOG_DBG("returning %s for position %lu", 
        (rm_->n2s_[index]).get(), index );
    return (rm_->n2s_[index]).get();
}

size_t         
RelationMeta::getColumnIndex( const char *colname ) const
{
    if(!colname)
    {
        PANIC( "RelationMeta::getColumnIndex(), null arg" );
    }
    charpmap_t::const_iterator e = rm_->s2n_.end();
    charpmap_t::const_iterator i = rm_->s2n_.find( colname );
    if( i==e )
    {
        char_ptr known_columns=getRelationHeader();
        PANICV( "relation [%s] column name [%s] not found. known columns are: %s", 
            getRelationName(), colname, known_columns.get());
    }
    return (*i).second ;
}

void           
RelationMeta::addColumn( const char *colname )
{
    if(!colname)
    {
        PANIC( "RelationMeta::addColumn(), null arg" );
    }
    char_ptr cp=ptr_strdup( colname );
    charpmap_t::iterator e = rm_->s2n_.end();
    charpmap_t::iterator i = rm_->s2n_.find( colname );
    if( i!=e )
    {
        PANICV( "RelationMeta::addColumn(), column name [%s] not unique", colname );
    }
    rm_->s2n_[ cp.get() ]=rm_->n2s_.size();
    rm_->n2s_.push_back( cp );
    APPLOG_DBG("%s at position %lu", colname, rm_->n2s_.size()-1);
}

void
RelationMeta::addColumns( list< char_ptr > &columns )
{
    list< char_ptr >::iterator i;
    list< char_ptr >::iterator e=columns.end();
    for( i=columns.begin(); i!=e; ++i )
    {
        addColumn( (*i).get() );
    }
}

RelationMeta::RelationMeta(const RelationMeta &rhs )
{
    rm_ = new RelationMetaPriv(*rhs.rm_);
}

RelationMeta & 
RelationMeta::operator=(const RelationMeta &rhs )
{
    if( this != &rhs && rm_ != rhs.rm_ )
    {
        delete rm_;
        rm_ = new RelationMetaPriv(*rhs.rm_);
    }
    return *this;
}

