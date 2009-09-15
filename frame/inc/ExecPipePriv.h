// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExecPipePriv.h
// Author:    matt Matthew Grosso
// Created:   Sun Nov 28 10:51:15 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: ExecPipePriv.h,v 1.1 1999/11/30 10:19:12 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef EXECPIPEPRIV_H
#define EXECPIPEPRIV_H 1

#include "ptr.h"
#include "ptr_strdup.h"
#include <vector>

class ExecPipe ;

class ExecPipePriv {
    friend class ExecPipe ;

    private:
    char_ptr execstr_buf ;
    ptr<int,ptr_array_deleter<int> > pipes ;
    char * path ;
    ptr< char *, ptr_array_deleter< char * > > exec_argv ;
};



#endif /* EXECPIPEPRIV_H */
