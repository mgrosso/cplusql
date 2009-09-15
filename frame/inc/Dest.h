// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Dest.h
// Author:    mgrosso 
// Created:   Sun Jul  7 23:21:44 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: Dest.h,v 1.4 2003/10/28 22:22:41 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DEST_H
#define DEST_H 1

#include "ptr.h"

class Dest
{

    protected:
        Dest();
    public:
        virtual ~Dest();
        virtual void put( const char *s )=0;
        virtual void put( const char *s, int slen )=0;
        virtual void flush(void)=0;
    private:
        //not implemented
        Dest( const Dest &rhs );
        Dest &operator=( const Dest &rhs );
};

typedef ptr< Dest > pDest ;

#endif /* DEST_H */
