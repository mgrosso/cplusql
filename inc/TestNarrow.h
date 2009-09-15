// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      TestNarrow.h
// Author:    mgrosso Matt Grosso
// Created:   Sat Feb 28 11:16:21 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: TestNarrow.h,v 1.1 2004/08/30 19:23:32 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef TESTNARROW_H
#define TESTNARROW_H 1

#include "tao/ORB.h"

class TestNarrow 
{
public:

    TestNarrow( int argc, char **argv);
	~TestNarrow();

private:
//not implemented
    TestNarrow(const TestNarrow &);
    TestNarrow & operator=(const TestNarrow &);

    void object_var_narrow_var(CORBA::ORB_var& orb_, char* url);
    void object_ptr_narrow_ptr(CORBA::ORB_var& orb_, char* url, bool release_object, bool release_narrowed);
    void object_ptr_narrow_var(CORBA::ORB_var& orb_, char* url, bool release_object);
    void object_var_narrow_ptr(CORBA::ORB_var& orb_, char* url, bool release_narrowed);
};

#endif /* TESTNARROW_H */
