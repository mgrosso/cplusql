#ident  "file_id $Id: test-glob.cpp,v 1.3 2004/09/24 14:37:40 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-glob.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 29 12:23:57 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-glob.cpp,v 1.3 2004/09/24 14:37:40 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdio.h>

#define NEED_GLOB_H
#include "frame_config.h"

int main( int argc, char **argv )
{
#ifdef HAVE_GLOB_H
    glob_t glb;
    int flags = GLOB_ERR | GLOB_NOSORT 
#ifdef HAVE_GNU_GLOB
        | GLOB_BRACE | GLOB_TILDE 
#endif
    ;
    int ret;
    if( 0!=(ret=glob( "/etc/rc.d/init.d/*", flags, NULL, &glb )))
    {
        return ret;
    }
    fprintf( stderr, "found %i\n", glb.gl_pathc );
    for( unsigned int i=0; i<glb.gl_pathc; ++i )
    {
        fprintf( stderr, "%s\n", glb.gl_pathv[ i] );
    }
    return 0;
#endif /* HAVE_GLOB_H */
    return 1;
}







