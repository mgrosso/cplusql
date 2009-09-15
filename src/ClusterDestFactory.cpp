#ident "file_id $Id: ClusterDestFactory.cpp,v 1.7 2005/10/17 11:18:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ClusterDestFactory.cpp
// Author:    
// Created:   
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ClusterDestFactory.cpp,v 1.7 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "cplusql.h"
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1

#include <stdlib.h>
#include "Dest.h"
#include "DestFile.h"
#include "ClusterDestBuffered.h"
#include "DestThreadedBuffer.h"
#include "ClusterDestFile.h"
#include "ClusterDestFactory.h"
#include "clusterioC.h"
#include "BaseException.h"

ClusterDestFactory::ClusterDestFactory( 
    clusterio::NameSpaceService_ptr nss, const clusterio::writer_id & writer_id
)
: _nss( nss ), _writer_id( writer_id )
{
    ;//noop
}

pDest
ClusterDestFactory::getFileDest( const char *filename )
{
    pDest gb( new ClusterDestFile( _nss, _writer_id, filename )); 
    return gb;
}

ClusterDestFactory::~ClusterDestFactory() 
{
    ;//noop
};

#endif
#endif
