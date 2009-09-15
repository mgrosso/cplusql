
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DirOpen.h
// Author:    mgrosso Matthew Grosso
// Created:   Mon Dec  1 02:12:45 EST 2003 on localhost.localdomain
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DirOpen.h,v 1.1 2004/01/05 20:21:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DIROPEN_H
#define DIROPEN_H 1

#include "BaseException.h"
#include "ptr.h"

template < class T >
class ptr_closedir_deleter {
    public:
    PTR_INLINE static void do_delete ( T *t ){
        (void)closedir( t );
    };
};

typedef ptr< DIR ,  ptr_closedir_deleter< DIR > > DIRptr ;

class DirOpen
{
    public:
    static DIRptr open( const char *dirname )
    {
        DIR *d=opendir( dirname );
        if(!d)
        {
            PANIC_S( "cant opendir()", dirname );
        }
        DIRptr dp( d );
        return dp;
    };
    private:
};



#endif /* DIROPEN_H */
