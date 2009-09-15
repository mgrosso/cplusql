
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Index.h
// Author:    mgrosso 
// Created:   Sun Aug 15 21:09:48 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: Index.h,v 1.2 2004/12/31 20:59:20 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef INDEX_H
#define INDEX_H 1

#include "AppLog.h"
#include <map>
#include <vector>
#include "ptr.h"
#include "hashFuncs.h"
using namespace std;

class IndexPriv ;
class Index 
{
    public:
    typedef const char * key_type ;
    typedef size_t value_type ;
    typedef pair< key_type, value_type > kvpair_type ;
    typedef multimap< key_type, value_type, cc_less_than >  index_type  ;
    class Iterator 
    {
        public:
        inline Iterator( Index::index_type::iterator &i );
        inline key_type     getKey();
        inline value_type & getValue();
        inline value_type & operator *();
        inline Iterator &   operator ++();
        inline Iterator     operator ++(int);
        inline bool         operator ==(const Iterator &rhs);
        inline bool         operator !=(const Iterator &rhs);
        private:
        index_type::iterator        current_ ;
        //we are using compiler built ins for default and copy ctor.
    };
    typedef pair< Iterator, Iterator > bounds_type ;

                        Index();
                        ~Index();
    void                add( key_type key,  size_t rownum );
    bounds_type         bounds( key_type key );
    Iterator            end();
    Iterator            begin();

    private:
    IndexPriv *d_;

    //not implemented
    Index( const Index &rhs );
    Index &operator=( const Index &rhs );
};

typedef ptr< Index > IndexPtr ;

Index::Iterator::Iterator( Index::index_type::iterator &i )
:current_(i)
{
    ;//noop
}

Index::key_type
Index::Iterator::getKey()
{
    //APPLOG_DBG("this=%lx", reinterpret_cast< long unsigned >(this));
    return (*current_).first ;
}

Index::value_type &
Index::Iterator::getValue()
{
    //APPLOG_DBG("this=%lx", reinterpret_cast< long unsigned >(this));
    return (*current_).second ;
}

Index::value_type &
Index::Iterator::operator *()
{
    //APPLOG_DBG("this=%lx", reinterpret_cast< long unsigned >(this));
    return (*current_).second ;
}

Index::Iterator &
Index::Iterator::operator++()
{
    //APPLOG_DBG("this=%lx", reinterpret_cast< long unsigned >(this));
    ++ current_;
    return *this ;
}

Index::Iterator
Index::Iterator::operator++(int)
{
    //APPLOG_DBG("this=%lx", reinterpret_cast< long unsigned >(this));
    Index::Iterator gb( current_ );
    ++ current_;
    return gb;
}

bool
Index::Iterator::operator==(const Index::Iterator &rhs )
{
    if( rhs.current_ == current_ )
    {
        return true;
    }
//    if((*rhs.current_).first && (*current_).first &&
//        !strcmp( (*rhs.current_).first , (*current_).first ))
//    {
//        APPLOG_DBG("this=%lx leftkey=%s rightkey=%s", 
//            reinterpret_cast< long unsigned >(this),
//            (*rhs.current_).first , (*current_).first );
//        return true;
//    }
//    APPLOG_DBG("this=%lx", reinterpret_cast< long unsigned >(this));
    return false;
}

bool
Index::Iterator::operator!=(const Index::Iterator &rhs )
{
    if( *this == rhs )
    {
        return false;
    }
    return true;
}

#endif /* INDEX_H */
