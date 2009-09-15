#ident "file_id $Id: ptr_strdup.cpp,v 1.14 2005/02/22 07:15:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ptr_strdup.cpp
// Author:    matt Matthew Grosso
// Created:   Fri Nov 26 23:11:18 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: ptr_strdup.cpp,v 1.14 2005/02/22 07:15:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ptr_strdup.h"
#include "BaseException.h"
#include "Convert.h"
#include <stdio.h>

char *
strdup_len( const char *in, size_t sz )
{
    if ( ! in ){
        PANIC( "strdup_wnew(): null argument " );
    }
    char *buf= new char [sz + 1];
    memcpy( buf, in, sz );
    buf[sz]='\0';
    return buf;
}

char *
strdup_wnew( const char *in )
{
    if ( ! in ){
        PANIC( "strdup_wnew(): null argument " );
    }
    int len=strlen(in)+1;
    char *buf= new char [len];
    strcpy( buf, in );
    buf[len-1]='\0';
    return buf;
}

char_ptr
ptr_strdup_take( char *in ){
    return char_ptr (in);
}

char_ptr
ptr_strdup( const char *in ){
    return char_ptr ( strdup_wnew(in));
}

char_ptr
ptr_strdup_len( const char *in, size_t sz  ){
    return char_ptr ( strdup_len(in, sz ));
}

char_ptr
ptr_strdup_plus( const char *lhs, const char *rhs ){
    size_t len=1;
    if( lhs )
    {
        len += strlen(lhs);
    }
    if( rhs )
    {
        len += strlen(rhs);
    }
    char *buf=new char[len];
    if( lhs )
    {
        strcpy(buf,lhs);
    } else
    {
        buf[0]='\0';
    }
    if( rhs )
    {
        strcat(buf,rhs);
    }
    return char_ptr(buf);
}

char_ptr
operator +( const char_ptr &lhs, const char_ptr &rhs ){
    return ptr_strdup_plus( lhs.get(), rhs.get());
}

char_ptr
operator +( const char *lhs, const char_ptr &rhs ){
    return ptr_strdup_plus( lhs, rhs.get());
}

char_ptr
operator +( const char_ptr &lhs, const char *rhs ){
    return ptr_strdup_plus( lhs.get(), rhs);
}


char_ptr
ptr_strdup_wnull( const char *in ){
    static const char * blank = "";
    if ( ! in) {
        in = blank;
    }
    return ptr_strdup( in );
};

char_ptr
ptr_strdup_longlong( long long i )
{
    char *buf = new char[32];
    Convert::LongLongToString( buf, i );
    return ptr_strdup_take( buf );
};

char_ptr
ptr_strdup_long( long i )
{
    char *buf = new char[32];
    Convert::LongToString( buf, i );
    return ptr_strdup_take( buf );
};

char_ptr
ptr_strdup_double( double d )
{
    char *buf = new char[32];
    Convert::DoubleToString( buf, d );
    return ptr_strdup_take( buf );
};

char_ptr
ptr_strdup_longdouble( long double d )
{
    char *buf = new char[64];
    Convert::LongDoubleToString( buf, d );
    return ptr_strdup_take( buf );
};

char_ptr
ptr_strdup_list( const charvec &cvp, const char *delim, bool trailing )
{
    int outsize=2;//newline + null is minimum
    int delim_len = strlen( delim );
    charvec::const_iterator i;
    charvec::const_iterator e;
    for( i=cvp.begin(), e=cvp.end(); i!=e; ++i )
    {
        outsize += strlen( (*i) );
        outsize += delim_len ;
    }
    if( trailing )
    {
        outsize += delim_len;
    }
    char *buf= new char [outsize];
    memset(buf,'\0',outsize);
    i=cvp.begin(); 
    e=cvp.end(); 
    if( i!= e )
    {
        strcat( buf, (*i++));
    }
    while( i!=e )
    {
        strcat( buf, delim );
        strcat( buf, (*i));
        ++i ;
    }
    if( trailing )
    {
        strcat( buf, delim );
    }
    buf[ outsize -1 ]='\0';
    char_ptr giveback = buf;
    return giveback ;
};

char_ptr
ptr_strdup_ptr_list( const char_ptr_list &cvp, const char *delim, bool trailing )
{
    charvec cv;

    char_ptr_list::const_iterator i;
    char_ptr_list::const_iterator e;
    for( i=cvp.begin(), e=cvp.end(); i!=e; ++i )
    {
        cv.push_back( (*i).get() );
    }
    return ptr_strdup_list ( cv, delim, trailing );
};

char_ptr
ptr_strdup_ptr_vec( const char_ptr_vec &cvp, const char *delim, bool trailing )
{
    charvec cv;
    char_ptr_vec::const_iterator i;
    char_ptr_vec::const_iterator e;
    for( i=cvp.begin(), e=cvp.end(); i!=e; ++i )
    {
        cv.push_back( (*i).get() );
    }
    return ptr_strdup_list ( cv, delim, trailing );
};

char_ptr
ptr_strdup_arrchar( char **arr, unsigned len, const char *delim, bool trailing )
{
    charvec cv;
    for( unsigned i=0; i <len; ++i )  
    {
        cv.push_back( arr[i] );
    }
    return ptr_strdup_list ( cv, delim, trailing );
};

char **
argv_new( unsigned size )
{
    char **s=new char *[size+1];
    memset( s, '\0', sizeof(char *)*(size+1));
    return s;
}

argv_ptr
argv_ptr_new( unsigned size )
{
    argv_ptr gb=argv_new( size );
    return gb;
}

void
argv_ptr_copy( argv_ptr &dest, const char **src, unsigned size )
{
    for( unsigned i=0; i< size ; ++i )
    {
        dest[i]=strdup_wnew(src[i]);
    }
}

argv_ptr
argv_ptr_strdup( const char **src, unsigned size )
{
    argv_ptr gb=argv_ptr_new( size );
    argv_ptr_copy( gb, src, size );
    return gb;
}

argv_ptr
argv_ptr_strdup( const argv_ptr &src )
{
    unsigned size = 0;
    while( src[size] )
    {
        ++size ;
    }
    return argv_ptr_strdup( const_cast< const char ** >(src.get()), size );
}

char_ptr
ptr_replace_all( const char *src, const char *key, const char *val )
{
    if( ! src || ! key || ! val )
    {
        PANIC("null arg");
    }
    size_t keysize = strlen( key );
    char_ptr giveback=ptr_strdup(src);
    const char *s=giveback.get();
    const char *nextkey;
    while(( nextkey = strstr(s, key)))
    {
        size_t tlen = strlen( s );
        size_t frontlen = nextkey - s;
        size_t backlen = tlen - frontlen - keysize ;
        char_ptr front = ptr_strdup_len( s, frontlen );
        char_ptr back = ptr_strdup_len(
            s+frontlen+keysize, backlen );
        char_ptr a = front + val;
        giveback = a + back ;
        s = giveback.get();
    }
    return giveback ;
}

ptr< char_ptr_list >
ptr_strdup_deepcopy( vector<char *> &v )
{
    ptr< char_ptr_list > gb = new char_ptr_list;
    size_t i, e;
    for( i=0, e=v.size(); i< e; ++i )
    {
        gb->push_back( ptr_strdup( v[i] ));
    }
    return gb;
}

