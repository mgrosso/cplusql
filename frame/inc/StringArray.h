
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArray.h
// Author:    mgrosso 
// Created:   Wed Jul  7 20:17:40 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArray.h,v 1.2 2004/08/11 21:42:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRINGARRAY_H
#define STRINGARRAY_H 1

#include <list>
#include "ptr_strdup.h"

using namespace std;

class StringArrayPriv ;
class StringArray
{
    public:
                    ~StringArray();
                    StringArray();
                    StringArray( bool leak );

    void            take( char *s );
    void            take( char_ptr &s );
    void            take( list< char_ptr > & lst );

    void            copy( const char *s );
    void            copy( const char_ptr &s );
    void            copy( const list< char_ptr > & lst );

    char *          get( size_t index );
    const char *    get( size_t index ) const ;
    size_t          size() const;

    private:
    void            init( bool leak );

    StringArrayPriv *d_;
    //not implemented
    StringArray( const StringArray &rhs );
    StringArray &operator=( const StringArray &rhs );
};

typedef ptr< StringArray > StringArrayPtr ;


#endif /* STRINGARRAY_H */
