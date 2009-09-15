
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CharIndex.h
// Author:    mgrosso Matthew Grosso
// Created:   Wed Feb 25 10:11:10 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: CharIndex.h,v 1.1 2004/03/03 17:21:25 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef CHARINDEX_H
#define CHARINDEX_H 1

#include "limits.h"

#define CHARINDEX_NOTFOUND -1

class CharIndexPriv ;
class CharIndex
{
    public:
    typedef unsigned char       key_t;
    typedef int                 index_t;

    CharIndex();
    ~CharIndex();

    index_t getIndex( key_t k ) const;
    index_t makeIndex( key_t k );
    index_t operator[]( key_t k );

    private:
    index_t                     arr_[ UCHAR_MAX ];
    index_t                     next_;

    //not implemented
    CharIndex( const CharIndex &rhs );
    CharIndex &operator=( const CharIndex &rhs );
};



#endif /* CHARINDEX_H */
