// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ClusterDestFile.h
// Author:    
// Created:   
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ClusterDestFile.h,v 1.6 2004/06/16 10:19:10 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CLUSTERDESTFILE_H
#define CLUSTERDESTFILE_H 1

#include "DestFile.h"
#include "clusterioC.h"

class ClusterDestFilePriv;

class ClusterDestFile : public Dest
{
    public:
    ClusterDestFile( 
        const clusterio::NameSpaceService_ptr nss, 
        const clusterio::writer_id writer_id,
        const char *filename
    );

    virtual void put( const char *s );
    virtual void put( const char *s, int slen );
    virtual void flush( void );

    // prevent compiler auto generation
    virtual ~ClusterDestFile();

    private:
    ClusterDestFilePriv *_helper;

    // prevent compiler auto generation
    ClusterDestFile();
    ClusterDestFile( const ClusterDestFile &rhs );
    ClusterDestFile &operator=( const ClusterDestFile &rhs );
};

#endif /* CLUSTERDESTFILE_H */
