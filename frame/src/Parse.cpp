#ident "file_id $Id: Parse.cpp,v 1.10 2004/02/12 21:16:46 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Parse.cpp
// Author:    Matt Grosso
// Created:   Sat Nov 14 16:46:18 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Parse.cpp,v 1.10 2004/02/12 21:16:46 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Parse.h"
#include "string.h"

vector< char *>
Parse::parse( char *lumpen, const char *sepchars ){
    char *left ;
    vector< char * > giveback ;
    if( ! lumpen || ! *lumpen )
        return giveback ;
    giveback.push_back( lumpen );
    for(
        left=strpbrk(lumpen,sepchars);
        ( left!=NULL ) && ( *left!='\0' ); 
        left=strpbrk(left,sepchars))
        { 
        *left='\0';
        if(*(++left)=='\0')
            return giveback ;
        giveback.push_back( left );
    }
    return giveback;
};

ptr< vector< char *> >
Parse::parse_ptr( char *lumpen, const char *sepchars, bool bebroken ){
    char *left ;
    static const char * empty="";
    ptr< vector< char * > > giveback( new vector< char * > () );
    if( ! lumpen || ! *lumpen )
        return giveback ;
    giveback->push_back( lumpen );
    for(
        left=strpbrk(lumpen,sepchars);
        ( left!=NULL ) && ( *left!='\0' ); 
        left=strpbrk(left,sepchars))
    {
        *left='\0';
        if(*(++left)=='\0')
        {
            if( !bebroken) 
            {
                giveback->push_back( (char *)empty );
            }
            return giveback ;
        }
        giveback->push_back( left );
    }
    return giveback;
};

ptr< vector< char *> >
Parse::parse_rfc822_ptr( char *lumpen ){
    //fprintf(stderr,"parse_rfc822\n" );
    static const char *from="\n\nFrom ";
    char *left ;
    ptr< vector< char * > > giveback( new vector< char * > () );
    if( ! lumpen || ! *lumpen )
        return giveback ;
    giveback->push_back( lumpen );
    left=lumpen;
    for(
        left=strstr(left,from);
        ( left!=NULL ) && ( *left!='\0' ); 
        left=strstr(left,from)
    )
    { 
        *left='\0';
        ++left;
        ++left;//advance to the 'F' in \n\nFrom
        giveback->push_back( left );
        //fprintf(stderr,"message %i: BEGIN %s\nEND\n\n", giveback->size(),left );
    }
    return giveback;
};

#include <stdio.h>
// A somewhat safer version
vector< string >
Parse::parseString( const char *lumpen, const char *sepchars )
{
    vector< string > giveback ;
    const char *beg = lumpen;
    const char *end = NULL;

    if( !beg || ! *beg )
    {
        return giveback ;
    }
    do 
    { 
        end = strpbrk( beg, sepchars );
        string nextelem;

        if (end == NULL)
        {
            nextelem.append(beg);
            giveback.push_back( nextelem );
            return giveback ;
        }

        nextelem.append(beg,end);
        giveback.push_back( nextelem );
        beg = ++end; 
    } 
    while (( end != NULL ) && ( *end ));
    return giveback;
};

// A copy of "A somewhat safer version".
// The maximum index in the vector will be, "counter - 1".
// This function will return the first "counter" items parsed in the line.
vector< string >
Parse::parseString( const char *lumpen, const char *sepchars, int counter )
{
    vector< string > giveback ;
    const char *beg = lumpen;
    const char *end = NULL;

    if( !beg || ! *beg )
    {
        return giveback ;
    }
    do
    {
        end = strpbrk( beg, sepchars );
        string nextelem;

        if (end == NULL)
        {
            nextelem.append(beg);
            giveback.push_back( nextelem );
            return giveback ;
        }

        nextelem.append(beg,end);
        giveback.push_back( nextelem );
        beg = ++end;
    }
    while (( end != NULL ) && ( *end ) && (counter-- != 1));
    return giveback;
};

void
Parse::chop_newline( char *s )
{
    char *newline=strchr( s, '\n' );
    if(
        newline && 
        ( newline == strrchr( s, '\n' )) &&
        ( strlen(s) -1 == static_cast<size_t>(newline - s)  )
    )
    {
        *newline='\0';
    }
}
