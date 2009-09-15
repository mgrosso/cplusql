#ident  "file_id $Id: test-slurp.cpp,v 1.3 2004/08/11 21:42:44 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      slurpmain.cpp
// Author:    Matt Grosso
// Created:   Sat Nov 14 16:25:58 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: test-slurp.cpp,v 1.3 2004/08/11 21:42:44 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "Slurp.h"
#include "BaseException.h"

int main( int argc, char **argv ){
    char *s = Slurp::slurpFile( "test_slurp.out.eng" );
    printf("%s", s );
    return 0;
};



