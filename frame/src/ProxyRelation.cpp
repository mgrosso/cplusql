#ident "file_id $Id: ProxyRelation.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyRelation.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Nov 10 00:11:55 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ProxyRelation.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ProxyRelation.h"
#include "ColumnExpression.h"
#include "BaseException.h"
#include <stdio.h>

class ProxyRelationPriv
{
    private:
    friend class ProxyRelation ;
    RelationMetaPtr         meta_;
    ExpressionList          allp_;
    Expression **           all_;
    ProxyRelationPriv()
    :all_(NULL)
    {
        ;//noop
    };
    void resetAll()
    {
        size_t as=allp_->size();
        if( as == 0 )
        {
            return;
        }
        if( all_ )
        {
            delete [] all_;
        }
        all_= new Expression *[ as ];
        for( size_t i=0; i< as; ++i )
        {
            all_[i] = (*allp_)[i].get();
        }
    };
};

ProxyRelation::ProxyRelation(
    RelationMetaPtr &meta,
    ExpressionList &all
)
{
    proxypriv_=new ProxyRelationPriv();
    if(!proxypriv_)
    {
        THROW_NOMEM;
    }
    proxypriv_->allp_=all;
    proxypriv_->resetAll();
    proxypriv_->meta_=meta;
}

ProxyRelation::ProxyRelation(
    RelationMetaPtr &meta
)
{
    proxypriv_=new ProxyRelationPriv();
    if(!proxypriv_)
    {
        THROW_NOMEM;
    }
    ExpressionList all=new expression_list_t();
    proxypriv_->allp_=all;
    proxypriv_->resetAll();
    proxypriv_->meta_=meta;
}

ProxyRelation::ProxyRelation(
)
{
    proxypriv_=new ProxyRelationPriv();
    if(!proxypriv_)
    {
        THROW_NOMEM;
    }
    ExpressionList all=new expression_list_t();
    proxypriv_->allp_=all;
    proxypriv_->resetAll();
    RelationMetaPtr meta=new RelationMeta();
    proxypriv_->meta_=meta;
}

ProxyRelation::~ProxyRelation()
{
    //fprintf(stderr, "~ProxyRelation top %x\n", (unsigned )this);
    delete proxypriv_;
    //fprintf(stderr, "~ProxyRelation bottom %x\n", (unsigned )this);
}

void
ProxyRelation::addExpression( ExpressionPtr &e )
{
    proxypriv_->allp_->push_back( e );
    proxypriv_->resetAll();
}

void
ProxyRelation::clearExpressions( )
{
    proxypriv_->allp_->clear();
    proxypriv_->resetAll();
}

char_ptr
ProxyRelation::getCharPtr() 
{
    char_ptr_list cpl;
    ExpressionListItr i;
    ExpressionListItr e;
    for( i=proxypriv_->allp_->begin(), e=proxypriv_->allp_->end(); i!=e ; ++i )
    {
        cpl.push_back( (*i)->getCharPtr());
    }
    return ptr_strdup_ptr_list( cpl, "|", false );
}

const char *
ProxyRelation::getRaw(size_t index)
{
    return proxypriv_->all_[index]->getRaw();
}

char_ptr
ProxyRelation::getCharPtr(size_t index) const 
{
    return proxypriv_->all_[index]->getCharPtr();
}

bool
ProxyRelation::getBool(size_t index) const 
{
    return proxypriv_->all_[index]->getBool();
}

long int
ProxyRelation::getLong(size_t index) const 
{
    return proxypriv_->all_[index]->getLong();
}

long long int
ProxyRelation::getLongLong(size_t index) const 
{
    return proxypriv_->all_[index]->getLongLong();
}

long double
ProxyRelation::getLongDouble(size_t index) const 
{
    return proxypriv_->all_[index]->getLongDouble();
}

double
ProxyRelation::getDouble(size_t index) const 
{
    return proxypriv_->all_[index]->getDouble();
}

const char *
ProxyRelation::getRelationName() const 
{
    return proxypriv_->meta_->getRelationName();
}

const char *
ProxyRelation::getColumnName( size_t i ) const 
{
    return proxypriv_->meta_->getColumnName( i );
}

size_t
ProxyRelation::getNumColumns() const 
{
    return proxypriv_->meta_->getNumColumns();
}

size_t
ProxyRelation::getColumnIndex( const char *col_name ) const
{
    return proxypriv_->meta_->getColumnIndex( col_name );
}

char_ptr
ProxyRelation::getRelationHeader() const
{
    return proxypriv_->meta_->getRelationHeader();
}

