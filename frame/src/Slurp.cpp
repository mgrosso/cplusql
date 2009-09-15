#ident "file_id $Id: Slurp.cpp,v 1.6 2004/01/05 20:21:30 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Slurp.cpp
// Author:    Matt Grosso
// Created:   Sat Nov 14 16:11:43 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Slurp.cpp,v 1.6 2004/01/05 20:21:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ptr_strdup.h"
#include "Slurp.h"
#include "BaseException.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <list>

char *
Slurp::slurpFile( const char *filename ){
    int fd;
    if(-1==(fd=open( filename, O_RDONLY ))){
        char err_msg[128];
        sprintf(err_msg, " File %s could not be opened for slurping", filename);
        perror(err_msg);
        return NULL;
    }
    return slurpFile( fd, true );
};

char *
Slurp::slurpFile( int fd, bool close_after ){
    off_t begin=lseek( fd, 0, SEEK_SET );
    if(begin==-1 ){
        if(errno==ESPIPE){
            return slurpSocket( fd, close_after );
        }
        else if(errno==EBADF){
            perror("bad file slurping... ");
        }
        else{
            perror("bad lseek slurping file: ");
        }
        return NULL;
    }
    off_t end=lseek( fd, 0, SEEK_END );
    lseek( fd, 0, SEEK_SET );
    int size=end;
    char *bucket=new char[size+1];
    memset( bucket, '\0', size );
    if(-1==read( fd, bucket, size ))
        perror("bad read while slurping file: ");
    *(bucket+size)='\0';
    if(close_after){
        if(-1==close(fd))
            perror("slurpFile error closing file");
    }
    return bucket;
};

#define SLURP_BUF 8192
char *
Slurp::slurpSocket( int fd, bool close_after ){
    //PANIC( "Slurp::slurpSocket() not implemented. sorry."); 
    int totalsize=0;
    int size_sofar=0;
    list< char_ptr > lcp;

    while( 1 )
    {
        char buf[ SLURP_BUF ];
        memset( buf, '\0', SLURP_BUF );
        int ret = read( fd, buf, SLURP_BUF );
        if( ret <= 0 )
            break;
        totalsize += ret;
        lcp.push_back( ptr_strdup( buf ));
    }
    char *gb = new char[ totalsize+1 ];
    memset( gb, '\0', totalsize+1 );

    while( ! lcp.empty())
    {
        char_ptr cp = lcp.front();
        lcp.pop_front();
        strncpy( gb+size_sofar, cp.get(), totalsize - size_sofar );
        size_sofar += strlen( cp.get());
    }
    return gb ;
};

