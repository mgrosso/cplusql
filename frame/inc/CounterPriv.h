// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      counter_priv.h
// Author:    mgrosso Matthew Grosso
// Created:   Wed May 22 07:45:44 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: CounterPriv.h,v 1.1 2002/06/08 22:22:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef COUNTER_PRIV_H
#define COUNTER_PRIV_H 1

#include <map>
#include "ptr.h"
#include "ptr_strdup.h"
#include "hashFuncs.h"

typedef map< pkey_t, counter * > key_counter_map_t;
typedef map< char_ptr, counter *, char_ptr_less_than > char_counter_map_t;

class counter_iterator_priv
{
public:
    ~counter_iterator_priv();
private:
    friend class counter_iterator;
    friend class counter;
    counter_iterator_priv();

    static const int numbuf_size = 40 ;
    char_ptr    value_;
    key_counter_map_t::iterator ki;
    char_counter_map_t::iterator ci;
    key_counter_map_t::iterator ke;
    char_counter_map_t::iterator ce;
    bool iskey;
};

class counter_priv
{
    private:
    friend class counter;

    counter_priv();
    ~counter_priv();

    counter_data_factory factory_;
    key_counter_map_t   *key_map_;
    char_counter_map_t  *char_map_;
    counter_data        *data_ ;

    //not implemented
    counter_priv( const counter_priv &rhs );
    counter_priv &operator=( const counter_priv &rhs );
};

#endif /* COUNTER_PRIV_H */
