// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ptr_strdup.h
// Author:    matt Matthew Grosso
// Created:   Fri Nov 26 23:09:22 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: ptr_strdup.h,v 1.10 2005/02/22 07:15:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PTR_STRDUP_H
#define PTR_STRDUP_H 1

using namespace std ;
#include "ptr.h"
#include <list>
#include <vector>

typedef ptr< char , ptr_array_deleter < char > > char_ptr ;
typedef ptr< char *, argv_deleter < char * > > argv_ptr ;
typedef list < const char * > charvec ;
typedef list < char_ptr > char_ptr_list ;
typedef vector < char_ptr > char_ptr_vec ;

char **
argv_new( unsigned size );

argv_ptr
argv_ptr_new( unsigned size );

void
argv_ptr_copy( argv_ptr &dest, const char **src, unsigned size);

argv_ptr
argv_ptr_strdup( const char **src, unsigned size);

argv_ptr
argv_ptr_strdup( const argv_ptr &rhs );

char *
strdup_wnew( const char *in );

char * 
strdup_len( const char *in, size_t sz );

char_ptr 
ptr_strdup_take( char *in );

char_ptr 
ptr_strdup( const char *in );

char_ptr 
ptr_strdup_len( const char *in, size_t sz );

char_ptr 
ptr_strdup_plus( const char *lhs, const char *rhs );

char_ptr 
operator +( const char_ptr &lhs, const char *rhs );

char_ptr 
operator +( const char_ptr &lhs, const char_ptr &rhs );

char_ptr 
operator +( const char *lhs, const char_ptr &rhs );

char_ptr 
ptr_strdup_wnull( const char *in );

char_ptr
ptr_strdup_longlong( long long i );

char_ptr
ptr_strdup_long( long i );

char_ptr
ptr_strdup_double( double d );

char_ptr
ptr_strdup_longdouble( long double d );

//if you dont want delimiters, then pass "" as the delim.
//trailing=true if you always want to end the line with a delimiter
char_ptr
ptr_strdup_list( const charvec &cvp, const char *delim, bool trailing  );

char_ptr
ptr_strdup_ptr_list( const char_ptr_list &cvp, const char *delim, bool trailing  );

//yes I know that ptr_strdup_ptr_* should probably be a template...
char_ptr
ptr_strdup_ptr_vec( const char_ptr_vec &cvp, const char *delim, bool trailing  );

char_ptr
ptr_strdup_arrchar( char **arr, unsigned len, const char *delim, bool trailing );

char_ptr
ptr_replace_all( const char *src, const char *key, const char *val );

ptr< char_ptr_list >
ptr_strdup_deepcopy( vector<char *> &v );

#endif /* PTR_STRDUP_H */
