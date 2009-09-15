#ident "file_id $Id: Counter.cpp,v 1.3 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      counter.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Wed May 22 03:18:27 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: Counter.cpp,v 1.3 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Counter.h"
#include "CounterPriv.h"
#include "hashFuncs.h"
#include "BaseException.h"


counter_priv::counter_priv()
{
    key_map_ = NULL;
    char_map_ = NULL;
    data_ = NULL;
}

counter_priv::~counter_priv()
{
}

counter::counter( counter_data_factory f )
{
    d_ = new counter_priv();
    d_->factory_=f;
}

counter::~counter( )
{
    if( d_->data_ )
        delete d_->data_;
    if( d_->key_map_ )
        delete d_->key_map_ ;
    if( d_->char_map_ )
        delete d_->char_map_ ;
    delete d_;
}

void 
counter::ensure_counter()
{
    if(! d_->data_ )
    {
        d_->data_ = d_->factory_();
    }
    if(! d_->data_ )
    {
        PANIC( "cant make counter_data ");
    }
}

counter &
counter::getKeyChild( pkey_t key ) 
{
    counter *target;
    if( ! d_->key_map_ )
    {
        if( d_->char_map_ )
        {
            PANIC("cant have both key and char children.");
        }
        target = new counter( d_->factory_ );
        d_->key_map_ = new key_counter_map_t() ;
        d_->key_map_->insert( pair< pkey_t, counter * >( key, target));
        return *target;
    }
    key_counter_map_t::iterator i = d_->key_map_->find(key);
    if( i==d_->key_map_->end() )
    {
        target = new counter( d_->factory_ );
        d_->key_map_->insert( pair< pkey_t, counter * >( key, target));
        return *target;
    }
    return *((*i).second);
}


counter &
counter::getCharChild( char_ptr str ) 
{
    counter *target;
    if( ! d_->char_map_ )
    {
        if( d_->key_map_ )
        {
            PANIC("cant have both key and char children.");
        }
        target = new counter( d_->factory_ );
        d_->char_map_ = new char_counter_map_t() ;
        d_->char_map_->insert( pair< char_ptr, counter * >( str, target));
        return *target;
    }
    char_counter_map_t::iterator i = d_->char_map_->find(str);
    if( i==d_->char_map_->end() )
    {
        target = new counter( d_->factory_ );
        d_->char_map_->insert( pair< char_ptr, counter * >( str, target));
        return *target;
    }
    return *((*i).second);
}

void
counter::addData( const counter_data *addme ) 
{
    ensure_counter();
    d_->data_->count( addme );
}

counter_data *
counter::getData() 
{
    ensure_counter();
    return d_->data_;
}

counter_iterator_priv::counter_iterator_priv()
:value_(ptr_strdup_wnull(NULL))
{
}

counter_iterator_priv::~counter_iterator_priv()
{
    ;//noop
}

counter_iterator::counter_iterator()
{
    d_ = new counter_iterator_priv();
}

counter_iterator::~counter_iterator()
{
    delete d_;
}

counter_iterator_p
counter::getIterator()
{
    counter_iterator_p gb( new counter_iterator());

    if( d_->char_map_ )
    {
        gb->d_->iskey= false;
        gb->d_->ce = d_->char_map_->end();
        gb->d_->ci = d_->char_map_->begin();
    }
    else if( d_->key_map_ )
    {
        gb->d_->iskey= true;
        gb->d_->ke = d_->key_map_->end();
        gb->d_->ki = d_->key_map_->begin();
    }else
    {
        PANIC("counter::getIterator(), leaf node cant give iterator.");
    }

    gb->setString();
    return gb;
}

counter &
counter_iterator::get()
{
    if( d_->iskey )
    {
        return *(*(d_->ki)).second;
    }
    return *(*(d_->ci)).second;
}

void
counter_iterator::setString()
{
    if( ! d_->iskey )
    {
        d_->value_ = (*(d_->ci)).first;
        return ;
    }
    pkey_t key = (*(d_->ki)).first;

    char numbuf[counter_iterator_priv::numbuf_size];
    memset( numbuf, '\0', counter_iterator_priv::numbuf_size );
    snprintf( numbuf, counter_iterator_priv::numbuf_size, "%Li", key );
    d_->value_ = ptr_strdup_wnull( numbuf );
}

bool
counter_iterator::next()
{
    if( d_->iskey )
    {
        if( ++(d_->ki)==d_->ke )
        {
            return false;
        }
    }
    else if( ++(d_->ci)==d_->ce )
    {
        return false;
    }
    setString();
    return true;
}

const char *
counter_iterator::getString()
{
    return d_->value_.get();
}

