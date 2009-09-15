
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CharMap.h
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb 23 22:55:18 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: CharMap.h,v 1.1 2004/03/03 17:21:25 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef CHARMAP_H
#define CHARMAP_H 1

#include "ptr_strdup.h"

class CharMapPriv<T> ;

template <class T>
class CharMap
{
public:

    CharMap();
    ~CharMap();
    void set_leak_mem( bool b );

    //this class provides a memory efficient mapping from keys of unsigned char
    //* to type T, for large numbers of keys.  for small numbers of keys, the
    //overhead of this class would be bit higher and you should use map<>
    //instead.  Implementation uses a map in which every character of every key
    //at position i of L is represented as a separate node at that level. the
    //result is a graph of depth L where L is the longest key.  If two keys
    //share the first 90 characters, and differ only at the 91st, then adding
    //the second key to the map will only use more memory for the 91st char
    //and its leaf node.  each node is an array of pointers with an index
    //size equal to or less than UNSIGNED_CHAR_MAX. The position of a character
    //in that index is determined by a map specific to that depth in the map. 
    //Thus, if all keys in the map have only 3 unique characters at position
    //10, level 10 nodes will have arrays of size 3, consuming 3*sizeof(T*).

    //note:
    //at some point this class should be redone with iterators, but not for now.

    // returns true if suceeded, false otherwise. 
    // if false, no side effect.
    bool insertCreate( const unsigned char *key, unsigned len, const T &value );

    // returns true if suceeded, false otherwise.
    // if false, then value will have be set = to the value in the container.
    bool insertUnique( const unsigned char *key, unsigned len, const T &value );
    
    // returns true if value existed and is replaced, false otherwise.
    // either way, postcondition is key will exist in map with value of value 
    bool insertReplace( const unsigned char *key, unsigned len, const T &value );

    //true if the key exists
    bool haveKey( const unsigned char *key, unsigned len );

    //puts value into dest if key exists. otherwise returns false;
    bool readValue( const unsigned char *key, unsigned len, T &value_dest );

    //"dest" must not exist before call and "from" must exist before the 
    //call or false is returned.  Otherwise, the value of "from" will be
    //moved to "dest", and "from" will no longer exist after call.
    bool mv( const unsigned char *key_from, unsigned len_from, 
            const unsigned char *key_dest, unsigned len_dest );

    //returns ptr managed char ** array of keys that begin with prefix.
    argv_ptr find( const unsigned char *prefix, unsigned prefix_len );

    //returns true if key was found. false otherwise.
    bool rm( const unsigned char *key, unsigned len );

    //returns number of matching keys removed. a key matches if it shares
    //the prefix.
    unsigned long find_rm( const unsigned char *prefix, unsigned prefix_len );

private:
    CharMapPriv *d_;

    //not implemented
    CharMap( const CharMap &rhs );
    CharMap &operator=( const CharMap &rhs );
};



#endif /* CHARMAP_H */
