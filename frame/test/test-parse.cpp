#ident  "file_id $Id: test-parse.cpp,v 1.4 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      parsemain.cpp
// Author:    Matt Grosso
// Created:   Sat Nov 14 17:14:28 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: test-parse.cpp,v 1.4 2004/01/05 20:06:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Slurp.h"
#include "Parse.h"
#include "BaseException.h"

int main( int argc, char **argv ){
    char *passw = Slurp::slurpFile( "/etc/passwd" );

    vector< char * > fields=Parse::parse( passw, ":\n" );
    for( unsigned int i=0; i<fields.size() ; ++i )
        printf("%i=%s\n", i, fields[i]);

    char *testString = "one|two|three|four";
    vector< string > string_fields = Parse::parseString( testString, "|" );
    for( unsigned int i=0; i<string_fields.size() ; ++i )
        printf("%i=%s\n", i, string_fields[i].c_str());
    printf("%s\n", testString);

    vector< string > string_fields2 = Parse::parseString( testString, "+" );
    for( unsigned int i=0; i<string_fields2.size() ; ++i )
        printf("%i=%s\n", i, string_fields2[i].c_str());
    printf("%s\n", testString);

    return 0;
};







