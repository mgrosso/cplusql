#ident "file_id $Id: test-dirtree.cpp,v 1.1 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-dirtree.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Fri Jan  2 20:33:45 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: test-dirtree.cpp,v 1.1 2004/01/05 20:06:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <exception>
#include <stdio.h>
#include <string.h>
#include <strings.h>
using namespace std; 
#include "DirTree.h"

//static const char *         dirtree_h_s = "DirTree.h" ;
//static const char *         dirtree_cpp_s = "DirTree.cpp" ;
static const char *         dirtree_h_s = "DirTree.h" ;
static const char *         dirtree_cpp_s = "DirTree.cpp" ;
static const char *         dirtree_test_s = "test-dirtree.cpp" ;

class TestDirAction : public DirAction 
{
    int                         dirtree_h ;
    int                         dirtree_cpp ;
    int                         dirtree_test ;

    void if_strcmp_incr( int *i, const char *needle, const char *haystack )
    {
        if( strstr( haystack, needle ))
        {
            ++(*i) ;
        }
    };

    public:
    TestDirAction()
    : dirtree_h(0), dirtree_cpp(0), dirtree_test(0)
    {
        ;//noop
    };

    virtual void callback( 
        const char *filename, const char *path, bool is_dir, unsigned size 
    )
    {
        fprintf( stderr, "filename: %s path: %s is_dir:%i size:%u\n", 
            filename, path, is_dir, size );
        if_strcmp_incr( &dirtree_h, dirtree_h_s, filename );
        if_strcmp_incr( &dirtree_cpp, dirtree_cpp_s, filename );
        if_strcmp_incr( &dirtree_test, dirtree_test_s, filename );
    };

    bool complete()
    {
        if(  dirtree_h == 1  && dirtree_cpp == 1 && dirtree_test == 1 )
        {
            return true;
        }
        fprintf( stderr, "TestDirAction: dirtree_h=%i dirtree_cpp=%i dirtree_test=%i\n", 
                dirtree_h, dirtree_cpp, dirtree_test );
        return false;
    };
};

int 
main( int argc, char **argv )
{
    try{
        TestDirAction tda;
        DirTree dt( ".." );
        dt.traverse( &tda, false, true );
        if( tda.complete())
        {
            return 0;
        }
        return 1;
    }catch ( exception &e )
    {
        fprintf( stderr, "exception caught in main: %s\n", e.what());
    }
    return 1;
}

