#ident  "file_id $Id: test-exclusion.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      exmain.cpp
// Author:    Matt Grosso
// Created:   Thu Oct 29 15:58:30 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: test-exclusion.cpp,v 1.2 2004/01/05 20:06:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include "Exclusion.h"
#include "Thread.h"

class exmain {
	public:
		Mutex m;
		Condition c;
};

frame_thread_return_t
func1( void *x ){
	exmain *ex = (exmain *)x;
	printf("func1\n");
	sleep( 5 );
	MutexGuard mg( ex->m );
	ex->c.signal();
	printf("func1 finished signaling, sleep for 5 more\n");
	sleep( 5 );
	printf("func1 finished sleeping, \n");
	return FRAME_THREAD_DEFAULT_RETURN;
};

frame_thread_return_t
func2( void *x ){
	exmain *ex = (exmain *)x;
	printf("func2\n");
	MutexGuard mg( ex->m );
	ex->c.wait( mg );
	printf("func2 finished waiting\n");
	return FRAME_THREAD_DEFAULT_RETURN;
};

int main( int argc, char **argv ){
	exmain *ex = new exmain();
	Thread t( func1, ex );
	MutexGuard mg( ex->m );
	ex->c.wait( mg );
	return 0;
};



