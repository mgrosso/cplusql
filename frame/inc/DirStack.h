
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DirStack.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov 29 20:01:53 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DirStack.h,v 1.1 2004/01/05 20:21:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DIRSTACK_H
#define DIRSTACK_H 1
    
class DirStackPriv ;
class DirStack
{
    public:
    DirStack( const char *dir );
    ~DirStack();
    const char * prevdir() const ;

    private:
    DirStackPriv *d_;
    //not implemented
    DirStack();
    DirStack( const DirStack &rhs );
    DirStack &operator=( const DirStack &rhs );
};



#endif /* DIRSTACK_H */
