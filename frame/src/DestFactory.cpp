#ident "file_id $Id: DestFactory.cpp,v 1.9 2004/12/31 20:51:00 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestFactory.cpp
// Author:    mgrosso 
// Created:   Mon Jul  8 11:44:12 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: DestFactory.cpp,v 1.9 2004/12/31 20:51:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdlib.h>
#include "Dest.h"
#include "DestFactory.h"
#include "DestFile.h"
#include "DestBuffered.h"
#include "DestThreadedBuffer.h"

DestFactory::DestFactory()
:close_on_delete_(true), o_excl_(false), o_trunc_(true)
{
    ;//noop
}

DestFactory::DestFactory( bool close_on_delete, bool o_excl, bool o_trunc )
:close_on_delete_(close_on_delete), o_excl_(o_excl), o_trunc_(o_trunc)
{
    ;//noop
}

DestFactory::~DestFactory()
{
    ;//noop
}

pDest
DestFactory::getFileDest( const char *filename )
{
    return pDest ( 
        new DestFile( 
            filename, close_on_delete_, o_excl_, o_trunc_ ));
}

