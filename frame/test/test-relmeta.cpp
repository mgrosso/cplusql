#ident  "file_id $Id: test-relmeta.cpp,v 1.2 2004/01/05 20:06:24 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-relmeta.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Nov  7 01:46:19 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-relmeta.cpp,v 1.2 2004/01/05 20:06:24 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include <stdio.h>
#include "RelationMeta.h"
#include "BaseException.h"


int 
main( int argc, char **argv )
{
    try{
        RelationMeta rm;
        rm.addColumn( "one" );
        rm.addColumn( "two" );
        rm.addColumn( "three" );
        rm.addColumn( "four" );
        rm.addColumn( "five" );
        rm.setRelationName( "kownt25");

        fprintf( stderr, "getRelationName() %s\n", rm.getRelationName());

        fprintf( stderr, "getColumnIndex(%s) %u\n", 
            "one", rm.getColumnIndex("one"));
        fprintf( stderr, "getColumnIndex(%s) %u\n", 
            "two", rm.getColumnIndex("two"));
        fprintf( stderr, "getColumnIndex(%s) %u\n", 
            "three", rm.getColumnIndex("three"));
        fprintf( stderr, "getColumnIndex(%s) %u\n", 
            "four", rm.getColumnIndex("four"));
        fprintf( stderr, "getColumnIndex(%s) %u\n", 
            "five", rm.getColumnIndex("five"));

        fprintf( stderr, "getColumnName(%u) %s\n", 0, rm.getColumnName(0));
        fprintf( stderr, "getColumnName(%u) %s\n", 1, rm.getColumnName(1));
        fprintf( stderr, "getColumnName(%u) %s\n", 2, rm.getColumnName(2));
        fprintf( stderr, "getColumnName(%u) %s\n", 3, rm.getColumnName(3));
        fprintf( stderr, "getColumnName(%u) %s\n", 4, rm.getColumnName(4));

        fprintf( stderr, "//this should fail...\n");
        try{
            fprintf( stderr, "getColumnName(%u) %s\n", 5, rm.getColumnName(5));
        }catch( BaseException &b1 )
        {
            fprintf( stderr, "Exception caught, good: %s\n", b1.getMessage());
        }
        fprintf( stderr, "//so should this ...\n");
        try{
            fprintf( stderr, "getColumnIndex(%s) %u\n", 
                "six", rm.getColumnIndex("six"));
        }catch( BaseException &b2 )
        {
            fprintf( stderr, "Exception caught, good: %s\n", b2.getMessage());
        }
    }catch( BaseException &be )
    {
        fprintf( stderr, "Exception caught in main(): %s\n", be.getMessage());
        return 1;
    }
    return 0;
}
