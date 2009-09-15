// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      counter.h
// Author:    mgrosso Matthew Grosso
// Created:   Wed May 22 07:45:47 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: Counter.h,v 1.2 2003/01/22 05:45:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef COUNTER_H
#define COUNTER_H 1

#include <map>
#include "ptr.h"
#include "ptr_strdup.h"
typedef long long int pkey_t ;

class counter_data
{
    public:
    virtual void count( const counter_data *rhs )=0;
};

class simple_counter_data : public counter_data
{
    public:
    simple_counter_data(int val=0):cnt_(val)
    {
    };
    virtual void count( const counter_data *rhs )
    {
        cnt_+=((simple_counter_data *)rhs)->cnt_;
    };
    int getCount(){ return cnt_; };
    private:
    int cnt_;
};

typedef counter_data * (*counter_data_factory)() ;
class counter;
class counter_priv;
class counter_iterator;
class counter_iterator_priv;

typedef ptr< counter_iterator > counter_iterator_p;

class counter_iterator
{
friend class counter;
public:
    ~counter_iterator();

    bool next();
    counter &get();
    const char *getString();

private:
    counter_iterator_priv *d_;
    counter_iterator();
    void setString();

    //not implemented
    counter_iterator(const counter_iterator &rhs);
    counter_iterator &operator=(const counter_iterator &rhs);
};


class counter
{
    public:
    counter( counter_data_factory factory );
    ~counter();

    counter &       getKeyChild( pkey_t key ) ;
    counter &       getCharChild( char_ptr str ) ;
    void            addData( const counter_data *rhs );
    counter_data *  getData() ;

    counter_iterator_p getIterator();

    friend class counter_iterator;

    private:
    void ensure_counter();
    counter_priv *d_;

    //not implemented
    counter();
    counter( const counter &rhs );
    counter &operator=( const counter &rhs );
};


typedef map< pkey_t, counter * > key_counter_map_t;
typedef map< char_ptr, counter *, char_ptr_less_than > char_counter_map_t;

#endif /* COUNTER_H */
