
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DirTree.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov 29 17:45:47 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DirTree.h,v 1.1 2004/01/05 20:21:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DIRTREE_H
#define DIRTREE_H 1

#include "DirAction.h"

class DirTreePriv ;
class DirTree
{
    public:
    DirTree( const char *root );
    ~DirTree();
    void traverse( DirAction *action, bool do_hidden, bool ignore_errors );

    private:
    DirTreePriv *d_;
    //not implemented
    DirTree();
    DirTree( const DirTree &rhs );
    DirTree &operator=( const DirTree &rhs );
};



#endif /* DIRTREE_H */
