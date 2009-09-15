#ident "file_id $Id: CharIndex.cpp,v 1.1 2004/03/03 17:21:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CharIndex.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Wed Feb 25 10:11:10 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: CharIndex.cpp,v 1.1 2004/03/03 17:21:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CharIndex.h"
#include "limits.h"

CharIndex::CharIndex()
:next_(0)
{
    for( index_t i=0; i< UCHAR_MAX ; ++i )
    {
        arr_[i]=CHARINDEX_NOTFOUND;
    }
}

CharIndex::~CharIndex()
{
}

CharIndex::index_t
CharIndex::getIndex( key_t k ) const 
{
    return arr_[k];
}

CharIndex::index_t
CharIndex::makeIndex( key_t k )
{
    if( CHARINDEX_NOTFOUND == arr_[k] )
    {
        arr_[k]==next_++;
    }
    return arr_[k];
}

CharIndex::index_t
CharIndex::operator[]( key_t k )
{
    return makeIndex( k );
}


