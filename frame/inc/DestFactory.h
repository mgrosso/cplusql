// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestFactory.h
// Author:    mgrosso 
// Created:   Sun Jul  7 23:21:44 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: DestFactory.h,v 1.6 2004/12/31 20:50:57 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DESTFACTORY_H
#define DESTFACTORY_H 1

#include "AbstractDestFactory.h"

//offered here for convenience
class DestFactory : public AbstractDestFactory
{
    public:
    DestFactory();
    DestFactory( bool close_on_delete, bool o_excl, bool o_trunc );
    virtual ~DestFactory();
    virtual pDest getFileDest( const char *filename );

    protected:

    private:
    bool close_on_delete_; 
    bool o_excl_; 
    bool o_trunc_ ;

    //not implemented.
    DestFactory( const DestFactory &rhs );
    DestFactory &operator=( const DestFactory &rhs );
};

#endif /* DESTFACTORY_H */
