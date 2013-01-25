
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      hashFuncs.h
// Author:    
// Created:   Wed May 24 17:17:09 EDT 2000
// Project:   Primary Knowledge C++ Library code
// Purpose:   Provide common function defs for STL hash_maps
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: hashFuncs.h,v 1.11 2004/01/05 20:21:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef HASHFUNCS_H
#define HASHFUNCS_H 1

using namespace std;

#define NEED_HASH_MAP
#include "frame_config.h"

#include <string>
#include <string.h>
#include "ptr.h"
#include "ptr_strdup.h"
#include "BasicHashAlg.h"
#include "NumberTypes.h"

struct string_hash
{
    inline size_t operator()(const string &aString) const
    {
        return BasicHashAlg::getHash(aString.c_str());
    }
};

struct strings_equal
{
    inline bool operator()(const string &s1, const string &s2) const
    {
        return s1 == s2;
    }
};

struct string_less_than
{
    inline bool operator()(const string &s1, const string &s2) const
    {
        return s1 < s2;
    }
};

struct char_ptr_less_than
{
    inline bool operator()( const char_ptr &lhs, const char_ptr &rhs ) const
    {
        if( 0 <= strcmp( lhs.get(), rhs.get()))
        {
            return false;
        }
        return true;
    };
};

struct cc_less_than
{
    inline bool operator()( const char *lhs, const char *rhs ) const
    {
        if( 0 <= strcmp( lhs, rhs))
        {
            return false;
        }
        return true;
    };
};

struct key_hash
{
    inline size_t operator()(const out_key_t &outKey) const
    {
        // This is slightly broken and should be modified
        // to actually hash the double without losing precision
        return BasicHashAlg::getHash( outKey );
    }
};

struct keys_equal
{
    inline bool operator()(const out_key_t &k1, const out_key_t &k2) const
    {
        return k1 == k2;
    }
};

typedef hash_map<string, string, string_hash, strings_equal> string_to_string_hash;
typedef hash_map<string, long, string_hash, strings_equal> string_to_long_hash;
typedef hash_map<long, string> long_to_string_hash;
typedef hash_map<int, long> result_to_long_hash;
typedef hash_map<long, bool> long_to_bool_hash;
typedef hash_map<out_key_t, bool, key_hash, keys_equal> key_to_bool_hash;
typedef hash_map<string, out_key_t, string_hash, strings_equal>
    string_to_key_hash;
typedef hash_map<out_key_t, string, key_hash, keys_equal> key_to_string_hash;

#endif /* HASHFUNCS_H */

