// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SimpleJoin.h
// Author:    mgrosso 
// Created:   Sun Jul  7 17:23:27 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: SimpleJoin.h,v 1.2 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SIMPLEJOIN_H
#define SIMPLEJOIN_H 1

#include "Dest.h"
#include "DelimitedSource.h"
#include "ptr.h"
#include "BaseException.h"

//typedef ptr< DelimitedSource > DelimitedSourcePtr ;

class SimpleJoinPriv ;

class SimpleJoin
{
    public:

    SimpleJoin();
    void loadMap( int keycol, DelimitedSourcePtr right );
    void processJoin( int keycol, DelimitedSourcePtr left,
        pDest pd, const char *output_delimiter 
    );
    ~SimpleJoin();

    private: 
    SimpleJoinPriv *d_ ;

    //not implemented
    SimpleJoin(const SimpleJoin &rhs );
    SimpleJoin & operator=(const SimpleJoin &rhs );
};


#endif /* SIMPLEJOIN_H */
