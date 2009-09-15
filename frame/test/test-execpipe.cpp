#ident  "file_id $Id: test-execpipe.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-execpipe.cpp
// Author:    matt Matthew Grosso
// Created:   Sun Nov 28 13:25:37 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: test-execpipe.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ExecPipe.h"
#include "unistd.h"


int main( int argc, char **argv ){

    ExecPipe ep("sort -r -n", " " );
    int epfd = ep.exec();
    write(epfd, "0\n",3 );
    write(epfd, "5\n",3 );
    write(epfd, "7\n",3 );
    write(epfd, "6\n",3 );
    write(epfd, "1\n",3 );
    write(epfd, "9\n",3 );
    write(epfd, "8\n",3 );
    write(epfd, "4\n",3 );
    write(epfd, "2\n",3 );
    write(epfd, "3\n",3 );
    ep.close();
    return 0;
}




