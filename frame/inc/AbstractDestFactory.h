// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractDestFactory.h
// Author:    
// Created:   
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: AbstractDestFactory.h,v 1.4 2004/06/30 12:14:06 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACTDESTFACTORY_H
#define ABSTRACTDESTFACTORY_H 1

#include "Dest.h"
#include "ptr.h"

//concrete factories can handle dest subclassing
class AbstractDestFactory
{
    public:
    virtual ~AbstractDestFactory();
    virtual pDest getFileDest( const char *filename )=0;

    protected:
    AbstractDestFactory();
};

typedef ptr< AbstractDestFactory > DestFactoryPtr ;

#endif /* ABSTRACTDESTFACTORY_H */
