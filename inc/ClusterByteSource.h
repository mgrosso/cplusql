
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ClusterByteSource.h
// Author:    mgrosso 
// Created:   Sun Apr 27 01:51:06 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ClusterByteSource.h,v 1.4 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef CLUSTERBYTESOURCE_H
#define CLUSTERBYTESOURCE_H 1
#include "cplusql.h"
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1

#include "ByteSource.h"
#include "clusterioC.h"

//need size_t
#include <sys/types.h>

class ClusterByteSourcePriv ; 

class ClusterByteSource : public ByteSource
{
    public:
    ClusterByteSource( clusterio::NameSpaceService_ptr nss, const char *filename );
    ClusterByteSource( clusterio::NameSpaceService_ptr nss, clusterio::open_info fd );
    
    virtual ~ClusterByteSource();

    virtual bool next( char *buffer, size_t &bytes_read, size_t bufsize );

    private:
    ClusterByteSourcePriv *d_;

    //not implemented
    ClusterByteSource();
    ClusterByteSource( const ClusterByteSource &rhs );
    ClusterByteSource &operator=( const ClusterByteSource &rhs );
};
#endif
#endif

#endif /* CLUSTERBYTESOURCE_H */
