// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ClusterDestFactory.h
// Author:    
// Created:   
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ClusterDestFactory.h,v 1.6 2004/06/30 12:20:25 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CLUSTERDESTFACTORY_H
#define CLUSTERDESTFACTORY_H 1

#include "AbstractDestFactory.h"
#include "clusterioC.h"

//offered here for convenience
class ClusterDestFactory : public AbstractDestFactory
{
    public:
    ClusterDestFactory( 
        clusterio::NameSpaceService_ptr nss, 
        const clusterio::writer_id & writer_id
    );

    virtual pDest getFileDest( const char *filename );

    virtual ~ClusterDestFactory();

    private:
    clusterio::NameSpaceService_ptr _nss;
    clusterio::writer_id _writer_id;

    //not implemented
    ClusterDestFactory();
    ClusterDestFactory( const ClusterDestFactory &rhs );
    ClusterDestFactory &operator=( const ClusterDestFactory &rhs );
};

#endif /* CLUSTERDESTFACTORY_H */
