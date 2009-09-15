#ident "file_id $Id: Index.cpp,v 1.2 2005/02/01 01:50:15 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Index.cpp
// Author:    mgrosso 
// Created:   Sun Aug 15 21:09:48 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: Index.cpp,v 1.2 2005/02/01 01:50:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Index.h"

class IndexPriv 
{
    private:
    friend class Index ;

    Index::index_type       index_ ;
};

Index::Index()
{
    d_=new IndexPriv ();
}

Index::~Index()
{
    delete d_;
}

void
Index::add( Index::key_type key, size_t rownum )
{
    d_->index_.insert( Index::index_type::value_type( key, rownum ));
}

Index::bounds_type
Index::bounds( Index::key_type key )
{
    pair< index_type::iterator, index_type::iterator > p = d_->index_.equal_range( key );
    bounds_type giveback ( p.first, p.second);
    return giveback ;
}

Index::Iterator
Index::begin()
{
    index_type::iterator b =  d_->index_.begin();
    Index::Iterator giveback( b );
    return giveback ;
}

Index::Iterator
Index::end()
{
    index_type::iterator e =  d_->index_.end();
    Index::Iterator giveback( e );
    return giveback ;
}

