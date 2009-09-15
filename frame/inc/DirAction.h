
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DirAction.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov 29 17:45:47 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   perform  abstract action for each file in dir.
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DirAction.h,v 1.1 2004/01/05 20:21:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DIRACTION_H
#define DIRACTION_H 1

#ifdef _WIN32 
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

class DirAction
{
    public:
    virtual void callback( const char *filename, const char *path, 
            bool is_dir, unsigned size )=0;
};

#endif /* DIRACTION_H */
