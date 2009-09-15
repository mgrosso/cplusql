// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StateQueues.h
// Author:    holahrei Howard Olah-Reiken
// Created:   Tue Feb 17 18:51:38 EST 2004 on bi_hor
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart Inc.  All Rights Reserved.
// 
// $Id: StateQueues.h,v 1.21 2004/05/25 19:44:12 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STATEMAPS_H
#define STATEMAPS_H 1

using namespace std;
#include <map>
#include "serviceC.h"
#include "ptr.h"
#include "ptr_strdup.h"
#include "JobQueueServiceS.h"
#include "ServiceStatusMap.h"
#include "hashFuncs.h"

typedef ptr<JobQueue::Job> JobPtr;
typedef list<JobPtr> StateQueue; 
typedef ptr<StateQueue> StateQueuePtr;

typedef map<const char_ptr, JobPtr, char_ptr_less_than> StateMap;
typedef ptr<StateMap> StateMapPtr;

class StateQueuesPriv;

class ScheduledHookEventHandler;
class NodeMinRunnable;

class StateQueues 
{
    friend class ScheduledHookEventHandler;
    friend class NodeMinRunnable;

    public:
        StateQueues(JobQueue::JobQueueRef* jobQueueRef);
        virtual ~StateQueues(void);
  
        MutexGuard* checkMutexGuard(const MutexGuard* existingMutexGuard);
        void printStateQueues (const char* message="", const MutexGuard* existingMutexGuard=NULL);

        void toReady(JobPtr& pJob);
        void toRunning(JobPtr& pJob);
        void runningToReady(JobPtr& pJob);
        void jobFinished(
            const JobQueue::ExecutionStatusStruct & executionStatus);
        void jobFailed( JobPtr& pJob );
        void reconfigure( const Service::NVPairList &config );

        void scheduleJobs(ServiceStatusMap* serviceStatusMap);
        JobQueue::JobStateIdList * waitForStatusList (const JobQueue::JobIdList & jobIds,
                                                      Service::time_t timeout);

        void confirmRunningJobs(ServiceStatusMap* serviceStatusMap);
        void killAllJobs(ServiceStatusMap* serviceStatusMap, int numThreads);
        void cleanupStaleOutput(ServiceStatusMap* serviceStatusMap, int numThreads);
        void initNodeMinRunnable(ServiceStatusMap* serviceStatusMap);

        void signalSchedulable();
        void waitForSchedulable();

    private:
        void killAllJobsInner(ServiceStatusMap* serviceStatusMap, int numThreads);

        int searchFinishedMap(const JobQueue::JobIdList & jobIds, JobQueue::JobStateIdList *returnList);
        void initializeScheduledQueue();

        StateQueuesPriv* d_;
};
#endif /* STATEMAPS_H */

