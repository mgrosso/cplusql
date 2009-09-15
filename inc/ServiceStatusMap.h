// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ServiceStatusMap.h
// Author:    holahrei Howard Olah-Reiken
// Created:   Tue Feb 17 18:51:38 EST 2004 on bi_hor
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ServiceStatusMap.h,v 1.9 2004/05/25 19:44:12 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SERVICESTATUSMAP_H
#define SERVICESTATUSMAP_H 1

using namespace std;
#include <map>
#include "serviceC.h"
#include "ptr.h"
#include "ptr_strdup.h"
#include "Exclusion.h"
#include "NodeCntl.h"

typedef ptr<Service::ServiceStatus> ServiceStatusPtr;
class ServiceStatusMapPriv;

class ServiceStatusMap 
{
    public:
        ServiceStatusMap(void);
        virtual ~ServiceStatusMap(void);
  
        MutexGuard* checkMutexGuard(const MutexGuard* existingMutexGuard);
        long size(const MutexGuard* existingMutexGuard=NULL);
        Service::ServiceStatusList* elements(const char* serviceName,
                                             const char* host,
                                             const MutexGuard* existingMutexGuard=NULL);
        void evaluateLiveness(int tolerance,const MutexGuard* existingMutexGuard=NULL);
        void printServiceStatusMap (const MutexGuard* existingMutexGuard=NULL);

        void pushStatusList (const Service::ServiceStatusList & serviceStatusList,
            const MutexGuard* existingMutexGuard=NULL);

        ServiceStatusPtr get(const char_ptr& pURL, 
            const MutexGuard* existingMutexGuard=NULL);

        void put(const char_ptr& pURL, ServiceStatusPtr& serviceStatusPtr, 
            const MutexGuard* existingMutexGuard=NULL);

        unsigned int
        countLive( const char *service, const NodeCntl *ncntl,
            const MutexGuard* existingMutexGuard );

        static void printServiceStatus(const Service::ServiceStatus *serviceStatus);

        void freeSlot(
            const char_ptr& pURL, 
            const MutexGuard* existingMutexGuard=NULL
        );

        static void modifyNode( 
            Service::ServiceStatus *jobexec, 
            int used, int available 
        );
        static void readNode(      
            Service::ServiceStatus *jobexec, 
            int &used, int &available
        );

        Mutex* getMutex() const;
    private:
        ServiceStatusMapPriv* d_;
};
#endif /* SERVICESTATUSMAP_H */

