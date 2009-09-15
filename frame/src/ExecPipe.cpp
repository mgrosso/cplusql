#ident "file_id $Id: ExecPipe.cpp,v 1.4 2004/08/11 21:42:43 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExecPipe.cpp
// Author:    matt Matthew Grosso
// Created:   Fri Nov 26 22:33:33 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: ExecPipe.cpp,v 1.4 2004/08/11 21:42:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ExecPipe.h"
#include "ExecPipePriv.h"
#include "BaseException.h"
#include "ptr.h"
#include "ptr_strdup.h"
#include <vector>
#include <unistd.h>
#include "Parse.h"



ExecPipe::ExecPipe( const char *execstr, const char *sepchar ){

    d_=new ExecPipePriv();
    //set up exec filename and args
    d_->execstr_buf = ptr_strdup( execstr );
    vector<char*> v_execstr=Parse::parse(d_->execstr_buf.get(),sepchar); 
    int vlen = v_execstr.size();
    if( vlen==0 ){
        PANIC( "ExecPipe::ExecPipe(): no args" );
    }
    //ok to leak exec_argv, seeing as how our stack is being erased by
    //execvp...
    d_->exec_argv = new char * [vlen] ;
    char **exec_argv=d_->exec_argv.get();
    exec_argv[vlen-1]=0;
    for( int i=1; i< vlen-1; ++i ){
        exec_argv[i-1]=v_execstr[i] ;
    }
    d_->path=v_execstr[0];

    d_->pipes=new int[2];
    d_->pipes.get()[0] = d_->pipes.get()[1] = -1 ;

}


int
ExecPipe::exec(){
    //create pipe, child sets stdin to output of pipe, parent returns 
    //fd for caller to write to.
    if( -1==pipe(d_->pipes.get()))PANIC( "ExecPipe::ExecPipe: pipe() failed" );

    int fk=fork();
    if(fk==-1)PANIC( "ExecPipe::ExecPipe: fork() failed" );
    if(fk==0){//child make stdin point to read part of pipe.
        if(-1==dup2( d_->pipes.get()[0], 0 ))
            PANIC( "ExecPipe::ExecPipe: dup2() failed" );
        if(-1==execvp( d_->path, d_->exec_argv.get() ))
            PANIC( "ExecPipe::ExecPipe: execvp() failed" );
        PANIC( "ExecPipe::exec: code after execvp() should not be reached!");
    } 
    return d_->pipes.get()[1];
}

void
ExecPipe::close(){
    if ( d_->pipes.get()[1] != -1 )
        if( -1== ::close( d_->pipes.get()[1] ))
            PANIC("ExecPipe::close() error closing childs stdin");
}

ExecPipe::~ExecPipe(){
    delete d_ ;
}


