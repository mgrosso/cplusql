// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Parse.h
// Author:    Matt Grosso
// Created:   Sat Nov 14 16:44:12 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Parse.h,v 1.8 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef PARSE_H
#define PARSE_H 1

#include <iostream>
#include <vector>
#include <string>
#include "ptr.h"

using namespace std;

class Parse {
    public:
    static vector< char * > parse( char *lumpen, const char *sepchars );

    // a somewhat faster version that doesnt copy the vector
    static ptr< vector< char * > > parse_ptr( char *lumpen, const char *sepchars, 
            bool bebroken = true );

    // parse an rfc822 mail box destructively. looks for \n\nFrom, sets first \n to 
    // NULL, puts ptr to 'F' on the vector.
    static ptr< vector< char * > > parse_rfc822_ptr( char *lumpen );

    // A somewhat safer version
    static vector< string > parseString( const char *lumpen, 
                                         const char *sepchars );

    // A copy of "A somewhat safer version".
    // The maximum index in the vector will be, "counter - 1".
    static vector< string > parseString( const char *lumpen, const char *sepchars, int counter );

    //iff s ends with newline, replace it with null.
    static void chop_newline( char *s );
};

#endif /* PARSE_H */
