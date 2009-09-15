#ident "file_id $Id: DirStack.cpp,v 1.2 2004/06/02 21:14:03 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DirStack.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov 29 20:01:55 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DirStack.cpp,v 1.2 2004/06/02 21:14:03 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "DirStack.h"
#include "BaseException.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

class DirStackPriv 
{
    private:
    friend class DirStack ;
    char     prevdir[ PATH_MAX ];
    DirStackPriv( const char *newdir )
    {
        memset( prevdir, '\0', PATH_MAX );
        if( NULL==getcwd(prevdir,PATH_MAX))
        {
            PANIC("cant getcwd()");
        }
        if( 0!=chdir( newdir ))
        {
            PANIC_S("cant chdir()", newdir );
        }
    };
    ~DirStackPriv()
    {
        if( prevdir[0] !='\0' && 0 != chdir( prevdir))
        {
            PANIC_S( "cant chdir() back" , prevdir );
        }
    };
};

const char *
DirStack::prevdir() const 
{
    return d_->prevdir;
}

DirStack::DirStack( const char *newdir )
{
    d_=new DirStackPriv ( newdir );
}

DirStack::~DirStack()
{
    delete d_;
}

