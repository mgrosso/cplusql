// -*- C++ -*-
//
// $Id: StateQueues.cpp,v 1.56 2004/10/01 04:37:17 mgrosso Exp $

//system headers
#include <stdio.h>
#include <map>
#include <set>

//frame headers
#include "ptr.h"
#include "StateQueues.h"
#include "Exclusion.h"
#include "BaseException.h"
#include "Exclusion.h"
#include "AppLog.h"
#include "Event.h"
#include "ThreadRunnable.h"
#include "DestFile.h"
#include "AppConfig.h"

//idl generated headers
#include "JobExecServiceS.h"

//cplusql service  classes headers
#include "keys.h"
#include "SharedServiceMain.h"
#include "NVPairUtil.h"
#include "ServiceStatusMap.h"
#include "NodeCntl.h"


class StateQueuesPriv
{
private:
    friend class StateQueues;

    StateQueuesPriv(JobQueue::JobQueueRef* jobQueueRef) : jobQueueRef(jobQueueRef)
    {
        ;//noop
    };

    JobQueue::JobQueueRef* jobQueueRef;

    StateQueue readyQueue;
    EventQueue* scheduledQueue;
    StateMap runningMap;
    StateMap finishedMap;

    Mutex lock_;
    Condition waitForStatusCondition_;
    Condition waitForOOSCondition_;
    Condition   schedulable_ ;
    NodeCntlPtr node_cntl_;

    void appendJobLocation( JobPtr &jp );

};

class JobExecUrlEvent { public:
	JobExecUrlEvent();
    JobExecUrlEvent(const char* jobExecUrl);
	~JobExecUrlEvent();
    char_ptr    jeptr ;
    const char* jobExecUrl_;
};

class CleanupStaleOutputEventHandler : public EventHandler{
    public:
    void handleEvent ( void *e );
};

class KillAllJobsEventHandler : public EventHandler{
    public:
    void handleEvent ( void *e );
};

class ScheduledEvent {
public:
	ScheduledEvent();
    ScheduledEvent(JobPtr pJob,
                   StateQueues* stateQueues );
	~ScheduledEvent();
    JobPtr pJob_;
    StateQueues* stateQueues_;
    JobQueue::JobQueueRef* jobQueueRef_;
};

class ScheduledEventHandler : public EventHandler{
    public:
    void handleEvent ( void *e );
};

class ScheduledHookEventHandler : public EventHandler{
    public:
    void handleEvent ( void *e );
};

class NodeMinRunnable : public Runnable
{
public:
    NodeMinRunnable( 
        Condition *cond, 
        Mutex *lock, 
        ServiceStatusMap *statmap, 
        const NodeCntl *n, 
        StateQueues *q );
    virtual ~NodeMinRunnable();
    virtual void    run();
private:
    Condition *cond_;
    Mutex *lock_;
    ServiceStatusMap *statmap_;
    const NodeCntl *n_;
    StateQueues *q_ ;
};

NodeMinRunnable::NodeMinRunnable( 
    Condition *cond, 
    Mutex *lock, 
    ServiceStatusMap *statmap, 
    const NodeCntl *n, 
    StateQueues *q 
)
:cond_(cond), lock_(lock), statmap_(statmap), n_(n), q_(q) 
{
    ;//noop
}

NodeMinRunnable::~NodeMinRunnable()
{
    ;//noop
}

void
NodeMinRunnable::run()
{
    while(1)
    {
        try{
            unsigned int nodemin ;
            unsigned int nodehave ;
            {
                MutexGuard mg(*lock_ );
                cond_->wait( mg );
                nodemin = AppConfig::instance()->getInt( 
                    JQS_MIN_NODES, "80" );
            }
            nodehave = statmap_->countLive( JE_NAME, n_, NULL );
            if( nodemin > nodehave )
            {
                MutexGuard mg(*lock_ );
                APPLOG_ABORT("Too many nodes oos. we have %u, and %s=%u. either adjust the parameter because these nodes really should be oos, or fix the thing thats causing us to take them oos and remove the latest .oos files. killing all jobs and exiting...",
                    nodehave, JQS_MIN_NODES, nodemin );
                try{
                    q_->killAllJobsInner( statmap_, 10 );
                }
                catch( std::exception &e )
                {
                    APPLOG_WARN( "NodeMinRunnable(): killAllJobs(): caught exception: %s", 
                        e.what());
                }
                catch(... )
                {
                    APPLOG_WARN( "NodeMinRunnable(): killAllJobs(): unknown exception");
                }
                exit(1);
            }
        }
        catch( std::exception &e )
        {
            APPLOG_WARN( 
                "NodeMinRunnable(): caught exception: %s", 
                e.what());
        }
        catch(... )
        {
            APPLOG_WARN( 
                "NodeMinRunnable(): unknown exception");
        }
    }
}


JobExecUrlEvent::JobExecUrlEvent(const char* jobExecUrl) 
   : jeptr(ptr_strdup( jobExecUrl )), jobExecUrl_(jeptr.get())
{
	APPLOG_DBG( "JobExecUrlEvent", "Ctor",(unsigned long int)this );
};

JobExecUrlEvent::~JobExecUrlEvent(){
	APPLOG_DBG( "JobExecUrlEvent", "Dtor",(unsigned long int)this);
};

void CleanupStaleOutputEventHandler::handleEvent (void *e)
{
    APPLOG_DBG("in CleanupStaleOutputEventHandler::handleEvent");

    JobExecUrlEvent* jobExecUrlEvent = (JobExecUrlEvent*)e;
    if( !e )
    {
        APPLOG_WARN("CleanupStaleOutputEventHandler::handleEvent(void *e)==NULL");
        return;
    }

    const char* jobExecUrl = jobExecUrlEvent->jobExecUrl_;

    try
    {
        CORBA::Object_ptr obj = 
            SharedServiceMain::instance()->resolve( jobExecUrl );
        if (obj == NULL)
        {
            APPLOG_WARN("JobExecService %s not resolved", jobExecUrl);
        }
        else
        {
            JobExec::JobExecService_ptr jobExecService = 
                JobExec::JobExecService::_narrow( obj );
            if (jobExecService == NULL)
            {
                APPLOG_WARN("JobExecService %s not found", jobExecUrl);
            }
            else
            {
                APPLOG_INFO("jobExecService->cleanupStale() on %s", 
                    jobExecUrl);
                jobExecService->cleanupStaleOutput();
            }
        }
    }
    catch (CORBA::Exception &ce) 
    {
        APPLOG_WARN("JobExecService %s not reached during CleanupStaleOutputEventHandler::cleanupStale. continuing", 
                    jobExecUrl);
    }
    catch (std::exception &e) 
    {
        APPLOG_WARN("Unable to cleanupStale on JobExecService %s.",
                    jobExecUrl);
        AppExceptionHandler::instance()->handleLogOnly( e );
    }
    catch ( ... )
    {
        APPLOG_WARN("Unable to cleanupStale on JobExecService %s. continuing",
                    jobExecUrl);
        AppExceptionHandler::instance()->handleLogOnly();
    }

    delete jobExecUrlEvent;
};

void KillAllJobsEventHandler::handleEvent (void *e)
{
    APPLOG_DBG("in KillAllJobsEventHandler::handleEvent");

    JobExecUrlEvent* jobExecUrlEvent = (JobExecUrlEvent*)e;
    if( !e )
    {
        APPLOG_WARN("KillAllJobsEventHandler::handleEvent(void *e)==NULL");
        return;
    }

    const char* jobExecUrl = jobExecUrlEvent->jobExecUrl_;

    try
    {
        CORBA::Object_ptr obj = 
            SharedServiceMain::instance()->resolve( jobExecUrl );
        if (obj == NULL)
        {
            APPLOG_WARN("JobExecService %s not resolved", jobExecUrl);
        }
        else
        {
            JobExec::JobExecService_ptr jobExecService = 
                JobExec::JobExecService::_narrow( obj );
            if (jobExecService == NULL)
            {
                APPLOG_WARN("JobExecService %s not found", jobExecUrl);
            }
            else
            {
                APPLOG_INFO("jobExecService->killAllRunning() on %s", 
                    jobExecUrl);
                jobExecService->killAllRunning();
            }
        }
    }
    catch (CORBA::Exception &ce) 
    {
        APPLOG_WARN("JobExecService %s not reached during KillAllJobsEventHandler::killAllJobs. continuing", 
                    jobExecUrl);
    }
    catch (std::exception &e) 
    {
        APPLOG_WARN("Unable to killAllRunning on JobExecService %s.",
                    jobExecUrl);
        AppExceptionHandler::instance()->handleLogOnly( e );
    }
    catch ( ... )
    {
        APPLOG_WARN("Unable to killAllRunning on JobExecService %s. continuing",
                    jobExecUrl);
        AppExceptionHandler::instance()->handleLogOnly();
    }

    delete jobExecUrlEvent;
};


void
StateQueuesPriv::appendJobLocation( JobPtr &jp )
{
    unsigned int i ;
    int newcnt = jp->nodesTried.length() + 1 ;
    JobQueue::JobExecRefList jobExecRefList(newcnt);
    jobExecRefList.length(newcnt);
    for( i=0; i < jp->nodesTried.length() ; ++i )
    {
        jobExecRefList[i] = jp->nodesTried[i] ;
    }
    jobExecRefList[i] = jp->currentNode;
    jp->nodesTried = jobExecRefList;
    jp->currentNode.url = "";
}

ScheduledEvent::ScheduledEvent(JobPtr pJob,
                               StateQueues* stateQueues )
   : pJob_(pJob), stateQueues_(stateQueues)
{
	APPLOG_DBG( "ScheduledEvent", "Ctor",(unsigned long int)this );
};

ScheduledEvent::~ScheduledEvent(){
	APPLOG_DBG( "ScheduledEvent", "Dtor",(unsigned long int)this);
};

void ScheduledEventHandler::handleEvent (void *e)
{
    APPLOG_DBG("in ScheduledEventHandler::handleEvent");

    ScheduledEvent* scheduledEvent = (ScheduledEvent*)e;
    if( !e )
    {
        APPLOG_WARN("ScheduledEventHandler::handleEvent(void *e)==NULL");
        return;
    }
    JobPtr pJob = scheduledEvent->pJob_;

    Service::ServiceRef ref ;
    SharedServiceMain::instance()->initServiceRef( ref );
    JobQueue::JobQueueRef* jobQueueRef = &ref ;

    char_ptr jobExecUrl=ptr_strdup(pJob->currentNode.url.in());
    char_ptr jobExecIor=ptr_strdup(pJob->currentNode.ior.in());

    bool jobRunning=false;
    try
    {
        if( pJob.get()==NULL || 
            pJob->jobSubmission.jobId.in() == NULL ||
            pJob->jobSubmission.args.command.in() == NULL ||
            jobExecIor.get() == NULL ||
            jobExecUrl.get() == NULL
        )
        {
            APPLOG_WARN("ScheduledEventHandler::handleEvent() null pointer");
            return;
        }

        APPLOG_INFO("Preparing to submit jobid %s command %s to jobexec %s, using ior %s", 
            pJob->jobSubmission.jobId.in(), 
            pJob->jobSubmission.args.command.in(),
            jobExecUrl.get(),
            jobExecIor.get());


        CORBA::Object_ptr obj = 
            SharedServiceMain::instance()->resolve( jobExecIor.get() );
        if (obj == NULL)
        {
            APPLOG_WARN("JobExecService %s not resolved using ior %s", jobExecUrl.get(), jobExecIor.get());
        }
        else
        {
            JobExec::JobExecService_ptr jobExecService = 
                JobExec::JobExecService::_narrow( obj );
            if (jobExecService == NULL)
            {
                APPLOG_WARN("JobExecService %s not found, using ior %s", jobExecUrl.get(), jobExecIor.get());
            }
            else
            {
                APPLOG_INFO("Submitting jobid %s command %s to jobexec %s", 
                    pJob->jobSubmission.jobId.in(), 
                    pJob->jobSubmission.args.command.in(),
                    jobExecUrl.get());
                jobRunning = jobExecService->runJob(
                    *jobQueueRef, pJob->jobSubmission); 
                APPLOG_INFO("runJob on %s returned %d",
                            jobExecUrl.get(), jobRunning);
            }
        }
    }
    catch (CORBA::Exception &ce) 
    {
        //AppExceptionHandler really needs to learn about CORBA...
        APPLOG_WARN("ScheduledEventHandler::handleEvent CORBA::Exception [%s][%s]", 
            ce._rep_id(), ce._name() );
    }
    catch (std::exception &e) 
    {
        AppExceptionHandler::instance()->handleLogOnly( e );
    }
    catch ( ... )
    {
        AppExceptionHandler::instance()->handleLogOnly();
    }

    if (!jobRunning) 
    {
        APPLOG_INFO("jobExecService %s was not able to run job %s.  Returning it to readyQueue",
                    jobExecUrl.get(), pJob->jobSubmission.jobId.in());
        scheduledEvent->stateQueues_->runningToReady(scheduledEvent->pJob_);
    }
    delete scheduledEvent;
};

void ScheduledHookEventHandler::handleEvent (void *e)
{
    void *f=e; 
    e=f;                //avoid compiler unused args warnings.
    //usleep(1000000);
}

StateQueues::StateQueues (JobQueue::JobQueueRef* jobQueueRef) 
{
    d_=new StateQueuesPriv(jobQueueRef);
    // Make sure node_cntl dir is there
    d_->node_cntl_ = SharedServiceMain::makeNodeCntl();
    initializeScheduledQueue();
}

void StateQueues::initNodeMinRunnable( ServiceStatusMap *statmap)
{
    RunnablePtr r = new NodeMinRunnable( 
        &d_->waitForOOSCondition_, 
        &d_->lock_, 
        statmap, 
        d_->node_cntl_.get(), 
        this );
    //we leak this for now. as long as shutdown is by kill() or exit()
    //this is not an issue.
    ThreadRunnable *t=new ThreadRunnable( r );
    t->start();
}

void StateQueues::initializeScheduledQueue()
{

    pEventFactory pEventFactory_;
    pEventHandler pScheduledHookEventHandler = new ScheduledHookEventHandler();
    pEventHandler pScheduledEventHandler = new ScheduledEventHandler();

    d_->scheduledQueue = new EventQueue(
        pEventFactory_, 
        pScheduledEventHandler, 
        pScheduledHookEventHandler,
        0,
        AppConfig::instance()->getInt( JQS_SUBMITQ_THR, "20"),
        AppConfig::instance()->getInt( JQS_SUBMITQ_MAX, "2147483640"));
}

// Implementation skeleton destructor
StateQueues::~StateQueues(void)
{
    delete d_;
}

MutexGuard* StateQueues::checkMutexGuard(const MutexGuard* existingMutexGuard)
{
    // this is duplicated from ServiceStatusMap class.  we should factor
    MutexGuard* mg=NULL;
    
    if (existingMutexGuard != NULL)
    {
        frame_mutex_t* existingMutex = existingMutexGuard->getMutex();
        if (existingMutex != d_->lock_.getMutex())
        {
            PANICV("invalid passed lock. mutex != d_->lock_.getMutex()");
        }
    }
    else
    {
        mg = new MutexGuard(d_->lock_);
    }
    return mg;
}
void StateQueues::printStateQueues (const char* message, const MutexGuard* existingMutexGuard)
{
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);
    
    APPLOG_INFO("%-30s:%3d jobs ready,%3d jobs scheduled,%3d jobs running,%3d jobs finished%s",
                message,
                d_->readyQueue.size(),
                d_->scheduledQueue->size(),
                d_->runningMap.size(),
                d_->finishedMap.size(),
                (d_->finishedMap.size() > 0 ? " - status detail follows:" : ""));

    StateMap::iterator current;
    StateMap::iterator end;
    for (current = d_->runningMap.begin(), end = d_->runningMap.end();
         current != end;
         ++current) 
    {
        APPLOG_INFO("runningq: %s: %s (%d)", 
            (*current).first.get(), 
            (*current).second->currentNode.url.in(), 
            (*current).second->jobState);
    }
 
    for (current = d_->finishedMap.begin(), end = d_->finishedMap.end();
         current != end;
         ++current) 
    {
        APPLOG_INFO("finishedq: %s: %s (%d)", 
            (*current).first.get(), 
            (*current).second->currentNode.url.in(),
            (*current).second->jobState);
    }
}

void StateQueues::toReady(JobPtr& pJob)
{
    MutexGuard mutexGuard(d_->lock_ );
    d_->readyQueue.push_back(pJob);
    signalSchedulable();
}

void StateQueues::runningToReady(JobPtr& pJob)
{
    MutexGuard mutexGuard(d_->lock_ );
    char_ptr pJobId = ptr_strdup(pJob->jobSubmission.jobId);
    d_->readyQueue.push_back(pJob);
    d_->runningMap.erase(pJobId);
    signalSchedulable();
    pJob->currentNode.url= "";
    pJob->currentNode.ior= "";
    pJob->currentNode.hostname= "";
    printStateQueues("after to runningToReady", &mutexGuard);
}

void StateQueues::scheduleJobs(ServiceStatusMap* serviceStatusMap)
{
    MutexGuard mutexGuard(d_->lock_ );

    APPLOG_DBG("calling serviceStatusMap->elements(\"JobExecService\", \"*\"");
    ptr<Service::ServiceStatusList> pServiceStatusList = 
        serviceStatusMap->elements(
            JE_NAME, "*", NULL );

    StateQueue tempQueue;
    if( ! pServiceStatusList->length() )
    {
        APPLOG_INFO("scheduleJobs(): no nodes available"); 
        return ;
    }
    while (!d_->readyQueue.empty())
    {
        JobPtr pJob = d_->readyQueue.front();
        APPLOG_DBG("pJob->jobSubmission.args.command=%s", 
            pJob->jobSubmission.args.command.in());

        bool exclusive = pJob->jobSubmission.args.exclusive;
        ConfigPtr jobc = NVPairUtil::convert( 
            pJob->jobSubmission.args.configuration );
        const char *required_node = jobc->findString( JOB_REQUIRES_HOST );

        if( required_node && *required_node)
        {
            if( ! d_->node_cntl_->isOK(required_node) )
            {
                APPLOG_INFO ("required node %s is oos, failing job %s:%s", 
                             required_node,
                             pJob->jobSubmission.jobId.in(),
                             pJob->jobSubmission.args.command.in());

                pJob->jobState = JobQueue::failed;
                char_ptr pJobId = ptr_strdup(pJob->jobSubmission.jobId);
                d_->readyQueue.pop_front();
                d_->finishedMap[pJobId] = pJob;
                d_->waitForStatusCondition_.broadcast();

                continue;
            }
        }
        // Now look through pServiceStatusList, starting with
        // the least busy, then progressing to the ones that
        // are allready something, but might still have more room.

        // maxUsedSlots will be set to the real max during the first 
        //pass over the list
        int maxUsedSlots=0;

        Service::ServiceStatus* jobExecServiceStatus = NULL;

        for (int desiredUsedSlots=0; 
             desiredUsedSlots <= maxUsedSlots && 
                jobExecServiceStatus==NULL ; 
             ++desiredUsedSlots)
        {
            int length=pServiceStatusList->length();
        
            for (int jobExecIndex=0; 
                 jobExecIndex<length && jobExecServiceStatus==NULL;
                 ++jobExecIndex)
            {
                Service::ServiceStatus* serviceStatus =
                    &((*pServiceStatusList)[jobExecIndex]);

                if( required_node && 
                    *required_node && 
                    0 != strcmp( 
                        required_node, serviceStatus->ref.hostname.in())
                )
                {
                    APPLOG_DBG("node requires %s, not %s", 
                        required_node,
                        serviceStatus->ref.hostname.in()
                    );
                    continue;
                }

                APPLOG_DBG("checking %s", serviceStatus->ref.url.in());
                if (!serviceStatus->live)
                {
                    APPLOG_INFO( "Skipping %s, because live==false",
                        serviceStatus->ref.url.in());
                    continue ;
                }
                if (serviceStatus->isPendingShutdown)
                {
                    APPLOG_INFO(
                        "Skipping %s, because isPendingShutdown==true",
                        serviceStatus->ref.url.in());
                    continue ;
                }

                int usedSlots, availableSlots;
                ServiceStatusMap::readNode( serviceStatus, usedSlots, availableSlots );

                if (availableSlots < 0 || usedSlots < 0)
                {
                    APPLOG_WARN("mangled status from %s", 
                        serviceStatus->ref.url.in());
                    continue ;
                }

                if (usedSlots != desiredUsedSlots || availableSlots == 0)
                {
                    if (!exclusive && usedSlots > maxUsedSlots)
                    {
                        // if the job is exclusive the maxUsedSlots 
                        // is always zero, else we increase it here
                        maxUsedSlots = usedSlots;
                    }
                    continue ;
                }

                if( ! d_->node_cntl_->isOK( 
                    serviceStatus->ref.hostname ))
                {
                    APPLOG_INFO ("%s is oos, keep looking", 
                        serviceStatus->ref.hostname.in() );
                    continue;
                }

                // we found a JobExec for this job
                // increment that JobExec's JE_USED_SLOTS
                // and decrement that JobExec's JE_AVAILABLE_SLOTS
                ServiceStatusMap::modifyNode( serviceStatus, 
                    ++usedSlots, --availableSlots );


                jobExecServiceStatus = 
                    new Service::ServiceStatus(*serviceStatus);
                char_ptr pURL = 
                    ptr_strdup(serviceStatus->ref.url);
                ServiceStatusPtr pServiceStatus(jobExecServiceStatus);
                serviceStatusMap->put(pURL, pServiceStatus, NULL );

                pJob->currentNode = serviceStatus->ref;
                char_ptr pJobId = ptr_strdup(pJob->jobSubmission.jobId);
                
                d_->readyQueue.pop_front();
                d_->runningMap[pJobId] = pJob ;

                ScheduledEvent* scheduledEvent =
                    new ScheduledEvent(pJob,
                                       this);
                d_->scheduledQueue->push(scheduledEvent);
            }
        }
        if (jobExecServiceStatus == NULL)
        {
            //There was no place for this job to run this time
            //Move it to the tempQueue
            APPLOG_INFO("There was no place for this job to run this time");
            d_->readyQueue.pop_front();
            tempQueue.push_back(pJob);
            if( ! required_node )
            {
                break; // no nodes left. 
            }
        }
    }
    if (!d_->readyQueue.empty())
    {
        tempQueue.splice( tempQueue.end(), d_->readyQueue);
    }
    d_->readyQueue=tempQueue;
}

void StateQueues::jobFinished (
    const JobQueue::ExecutionStatusStruct & executionStatus)
{
    char_ptr pJobId = ptr_strdup(executionStatus.jobId.in());

    MutexGuard mutexGuard(d_->lock_ );

    int count=d_->runningMap.count(pJobId);
    if (count == 0)
    {
        APPLOG_INFO("Job %s was not found in the runningMap.  Ignoring", 
            pJobId.get());
        return ;
    }
    JobPtr pJob = d_->runningMap[pJobId];

    APPLOG_DBG("d_->runningMap[%s]=%s",
        pJobId.get(), pJob->jobSubmission.args.command.in());


    if (executionStatus.executionStatus == JobQueue::succeeded_on_node)
    {
        APPLOG_INFO("%s succeeded!", pJobId.get());

        d_->appendJobLocation( pJob );
        pJob->jobState = JobQueue::succeeded;
        d_->finishedMap[pJobId] = pJob;
        d_->runningMap.erase(pJobId);
    }
    else
    {
        jobFailed(pJob );
        printStateQueues("after jobFailed returns", &mutexGuard);
    }
    APPLOG_DBG("d_->waitForStatusCondition.broadcast()");
    signalSchedulable();
    d_->waitForStatusCondition_.broadcast();
}

void StateQueues::jobFailed( JobPtr& pJob )
{
    char_ptr pJobId = ptr_strdup(pJob->jobSubmission.jobId.in());
    APPLOG_INFO("%s failed! pJob->nodesTried.length()=%d", 
        pJobId.get(), pJob->nodesTried.length());

    d_->appendJobLocation( pJob );

    if (pJob->nodesTried.length() == 1)//allready appended.
    {
        // that was the first attempt
        // oos the node and try again
        d_->node_cntl_->setOOS( pJob->currentNode.hostname.in(), 
            pJobId.get(), pJob->jobSubmission.args.command.in() );
        pJob->jobState = JobQueue::ready;
        d_->readyQueue.push_back(pJob);
        d_->runningMap.erase(pJobId);
        d_->waitForOOSCondition_.signal();
        return;
    }
    //length was 2
    const char *host = pJob->nodesTried[0].hostname.in();
    APPLOG_INFO("job %s failed twice, releasing first node %s", 
        pJobId.get(), host );

    pJob->jobState = JobQueue::failed;
    d_->finishedMap[pJobId] = pJob;
    d_->runningMap.erase(pJobId);

    d_->node_cntl_->setOK( host );
}

JobQueue::JobStateIdList * StateQueues::waitForStatusList (
                    const JobQueue::JobIdList & jobIds,
                    Service::time_t timeout)
{
    // Add your implementation here
    APPLOG_DBG("in waitForStatus, timeout=%d", timeout);

    int length = jobIds.length();
/*
    JobQueue::JobStateIdList *jobStateIdList =
        new JobQueue::JobStateIdList(
            length, length, new JobQueue::JobStateId[length], 1);
*/
    JobQueue::JobStateIdList *jobStateIdList =
        new JobQueue::JobStateIdList(length);
    jobStateIdList->length(length);
    
    if( length == 0 )
    {
        return jobStateIdList ;
    }

    APPLOG_DBG("waitForStatusList: jobStateIdList->length()=%d", length);
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    time_t entrySecondsSinceEpoch=tv.tv_sec;
    MutexGuard mg(d_->lock_);

    // notTimedOut will either be set based on time calculation (prior to
    // entry and at before completing each loop) or by 
    // Condition.timedwait
    // it will be false if it took too long to get the lock
    // rechecking current time, in case lock took a while to acquire
    gettimeofday(&tv, &tz);
    int elapsed=tv.tv_sec - entrySecondsSinceEpoch;
    int finishedCount = searchFinishedMap(jobIds, jobStateIdList);
    bool notTimedOut = (timeout == 0 || (elapsed < timeout));

    APPLOG_DBG("timeout=%d, elapsed=%d, notTimedOut=%d, finishedCount=%d, length=%d, (notTimedOut && finishedCount < length) == %d", 
        timeout,
        elapsed,
        notTimedOut,
        finishedCount,
        length,
        (notTimedOut && finishedCount < length)
    );

    while (notTimedOut && finishedCount < length)
    {
        APPLOG_DBG("timeout=%d, elapsed=%d, notTimedOut=%d, finishedCount=%d, length=%d, (notTimedOut && finishedCount < length) == %d", 
            timeout,
            elapsed,
            notTimedOut,
            finishedCount,
            length,
            (notTimedOut && finishedCount < length)
        );
        if (!timeout) // timeout of zero means do not time out
        {
            APPLOG_DBG("timeout of 0 specified, meaning indefinite, so d_->waitForStatusCondition_.wait(mg);");

            d_->waitForStatusCondition_.wait(mg);
            APPLOG_DBG("returned from d_->waitForStatusCondition_.wait(mg);");
            notTimedOut=false; //only wait once
        }
        else
        {
            APPLOG_DBG("timedwait( timeout=%d - elapsed=%d )", 
                timeout, elapsed );

            notTimedOut=d_->waitForStatusCondition_.timedwait(
                mg, timeout - elapsed, 0);

            APPLOG_DBG("returned from timedwait() %d", notTimedOut);
        }

        APPLOG_DBG("(timeout==0 || notTimedOut) == (%d==0 || %d) == %d", 
            timeout, notTimedOut, (timeout==0 || notTimedOut));

        if(timeout==0 || notTimedOut)
        {
            // we are either in a call where there is no timeout specified
            // or we have been awakened before the timeout period
            finishedCount = searchFinishedMap(jobIds, jobStateIdList);

            if(timeout != 0) // meaning a timeout was specified
            {
                gettimeofday(&tv, &tz);
                time_t now = tv.tv_sec; 
                elapsed = now - entrySecondsSinceEpoch ;
                notTimedOut = now < timeout;

                APPLOG_DBG("now=%d start=%d elapsed=%d timeout=%d finishedCount=%d length=%d notTimedOut=%d",
                    now,
                    entrySecondsSinceEpoch, 
                    elapsed,
                    timeout, 
                    finishedCount, 
                    length, 
                    notTimedOut
                );
            }
        }
        else
        {
            APPLOG_DBG("timed out");
        }
    }

    length = jobStateIdList->length();
    APPLOG_DBG("waitForStatusList: length()=%d", length);
    for (int i=0; i<length; ++i) 
    {
        APPLOG_DBG("(*jobStateIdList)[%d]=%d", i, (*jobStateIdList)[i]);
    }
    return jobStateIdList;
}

int 
StateQueues::searchFinishedMap(const JobQueue::JobIdList & jobIds, JobQueue::JobStateIdList *jobStateIdList)
{
    APPLOG_DBG("Entering StateQueues::searchFinishedMap");
    int length = jobIds.length();

    APPLOG_DBG("length = %d",length);
    int finishedCount=0;
    for (int i=0; i<length; ++i)
    {
        APPLOG_DBG("jobIds[%d]=%s, mapsize=%d", i, jobIds[i].in(),
            d_->finishedMap.size());

        char_ptr pJobId = ptr_strdup(jobIds[i].in());
        JobPtr pFinishedJob;

        if(//did a previous call allready find this job 
            ((*jobStateIdList)[i] == JobQueue::succeeded ) ||
            ((*jobStateIdList)[i] == JobQueue::failed )
        )
        {
            ++finishedCount;
        }
        else if(d_->finishedMap.count(pJobId) > 0)
        {
            pFinishedJob = d_->finishedMap[pJobId];
            APPLOG_DBG("was finished : %s, %d", 
                pJobId.get(), pFinishedJob->jobState);

            (*jobStateIdList)[i] = pFinishedJob->jobState;
            d_->finishedMap.erase(pJobId);
            ++finishedCount;
        } else if (d_->runningMap.count(pJobId) > 0 )
        {
            (*jobStateIdList)[i] = JobQueue::running;
            APPLOG_DBG("was running: %s", pJobId.get() );
        }
        else
        {
            StateQueue::iterator end=d_->readyQueue.end();
            bool found=false;
            for (StateQueue::iterator current = d_->readyQueue.begin();
                 current != end && !found;
                 ++current )
            {
                if (!strcmp((*current)->jobSubmission.jobId, pJobId.get()))
                {
                    (*jobStateIdList)[i] = JobQueue::ready;
                    found=true;
                }
            }
        
            if (!found)
            {
                Service::ServiceException err;
                char_ptr whatString = 
                    ptr_strdup_plus("Job Id not found: ", pJobId.get());
                err.what_string = (const char*)whatString.get();
                APPLOG_WARN((const char*)whatString.get());
                throw err;
            }
        }
    }
    return finishedCount;
}

void StateQueues::confirmRunningJobs(ServiceStatusMap* serviceStatusMap)
{
    MutexGuard mg(d_->lock_);

    StateMap::iterator end = d_->runningMap.end();
    for(StateMap::iterator current = d_->runningMap.begin();
        current != end;
        ++current)
    {
        JobPtr pJob = (*current).second;
        char_ptr pJobId = ptr_strdup(pJob->jobSubmission.jobId.in());

        APPLOG_DBG("confirming job(id=%s, command=%s ) node=%s", 
            pJob->jobSubmission.jobId.in(), 
            pJob->jobSubmission.args.command.in(), 
            pJob->currentNode.url.in()
            );

        char_ptr pURL = ptr_strdup( pJob->currentNode.url.in());
        ServiceStatusPtr pServiceStatus = serviceStatusMap->get(pURL,NULL);
        APPLOG_DBG("pServiceStatus.get()=%ux", pServiceStatus.get());
        if (pServiceStatus.get() == NULL ||
            pServiceStatus->live == false)
        {
            APPLOG_WARN("job(id=%s, command=%s ) on dead node=%s", 
                pJob->jobSubmission.jobId.in(), 
                pJob->jobSubmission.args.command.in(), 
                pJob->currentNode.url.in()
                );
            jobFailed(pJob );
        }
        else
        {
            APPLOG_DBG("confirmed, job node is live");
        }
    }
    signalSchedulable();
}

void StateQueues::killAllJobs(ServiceStatusMap* serviceStatusMap, 
                              int numThreads)
{
    MutexGuard mg(d_->lock_);
    killAllJobsInner(serviceStatusMap, numThreads);
}

void StateQueues::killAllJobsInner(ServiceStatusMap* serviceStatusMap, 
                              int numThreads)
{
    char numThreadsAsChar[50];
    if (snprintf(numThreadsAsChar, 50, "%d", numThreads) == -1)
    {
        PANICV("overflow in numThreads parameter to killAllJobs (%d)",
               numThreads);
    }
    numThreadsAsChar[49] = '\0';

    pEventFactory pEventFactory_;
    pEventHandler pKillAllJobsEventHandler = new KillAllJobsEventHandler();

    
    EventQueue killAllJobsEventQueue(
        pEventFactory_, pKillAllJobsEventHandler, 0,

        AppConfig::instance()->getInt( JQS_SUBMITQ_THR, numThreadsAsChar),
        AppConfig::instance()->getInt( JQS_SUBMITQ_MAX, "2147483640"));

    // first move all jobs on readyQueue to finishedMap with a status of failed
    while (!d_->readyQueue.empty())
    {
        JobPtr pJob = d_->readyQueue.front();

        APPLOG_INFO("Job %s: %s being moved from readyQueue to finishedMap as a failed job",
                    pJob->jobSubmission.jobId.in(),
                    pJob->jobSubmission.args.command.in());

        d_->readyQueue.pop_front();

        pJob->jobState = JobQueue::failed;
        char_ptr pJobId = ptr_strdup(pJob->jobSubmission.jobId);
        d_->finishedMap[pJobId] = pJob;
    }

    // now shutdown scheduledQueue and reinitialize
    d_->scheduledQueue->gracefulShutdown();
    initializeScheduledQueue();

    APPLOG_DBG("calling serviceStatusMap->elements(\"JobExecService\", \"*\"");
    ptr<Service::ServiceStatusList> pServiceStatusList = serviceStatusMap->elements(JE_NAME, "*");

    int length = pServiceStatusList->length();

    for (int i=0; i<length; ++i)
    {
        const char* jobExecUrl = (*pServiceStatusList)[i].ref.url;
        APPLOG_DBG("Adding JobExecService %s to KillAllJobsEventQueue",
                   jobExecUrl);
                   
        JobExecUrlEvent* jobExecUrlEvent =
            new JobExecUrlEvent(jobExecUrl);
        killAllJobsEventQueue.push(jobExecUrlEvent);
    }
    
    // now copy all jobs on runningQueue to finishedMap with a status of failed
    StateMap::iterator end = d_->runningMap.end();
    for(StateMap::iterator current = d_->runningMap.begin();
        current != end;
        ++current)
    {
        JobPtr pJob = (*current).second;
        pJob->jobState = JobQueue::failed;
        char_ptr pJobId = ptr_strdup(pJob->jobSubmission.jobId);
        d_->finishedMap[pJobId] = pJob;
    }

    // erase contents of running map
    d_->runningMap.erase(d_->runningMap.begin(), d_->runningMap.end());

    APPLOG_DBG("d_->waitForStatusCondition_.broadcast()");
    d_->waitForStatusCondition_.broadcast();
}    

void StateQueues::cleanupStaleOutput(ServiceStatusMap* serviceStatusMap, 
                                     int numThreads)
{
    MutexGuard mg(d_->lock_);

    char numThreadsAsChar[50];
    if (snprintf(numThreadsAsChar, 50, "%d", numThreads) == -1)
    {
        PANICV("overflow in numThreads parameter to cleanupStaleOutput (%d)",
               numThreads);
    }
    numThreadsAsChar[49] = '\0';

    pEventFactory pEventFactory_;
    pEventHandler pCleanupStaleOutputEventHandler = new CleanupStaleOutputEventHandler();

    
    EventQueue cleanupStaleOutputEventQueue(
        pEventFactory_, pCleanupStaleOutputEventHandler, 0,

        AppConfig::instance()->getInt( JQS_SUBMITQ_THR, numThreadsAsChar),
        AppConfig::instance()->getInt( JQS_SUBMITQ_MAX, "2147483640"));

    APPLOG_DBG("calling serviceStatusMap->elements(\"JobExecService\", \"*\"");
    ptr<Service::ServiceStatusList> pServiceStatusList = serviceStatusMap->elements(JE_NAME, "*");

    int length = pServiceStatusList->length();

    for (int i=0; i<length; ++i)
    {
        const char* jobExecUrl = (*pServiceStatusList)[i].ref.url;
        APPLOG_DBG("Adding JobExecService %s to CleanupStaleOutputEventQueue",
                   jobExecUrl);
                   
        JobExecUrlEvent* jobExecUrlEvent =
            new JobExecUrlEvent(jobExecUrl);
        cleanupStaleOutputEventQueue.push(jobExecUrlEvent);
    }
}

void 
StateQueues::signalSchedulable()
{
    d_->schedulable_.signal();
}

void 
StateQueues::waitForSchedulable()
{
    MutexGuard mg(d_->lock_ );
    d_->schedulable_.timedwait( 
        mg, 
        AppConfig::instance()->getInt( 
            JQS_SCHEDULER_THREAD_SLEEPTIME, "10000000" ), 
        0
    );
}

void 
StateQueues::reconfigure( const Service::NVPairList &changes )
{
    MutexGuard mutexGuard(d_->lock_ );
    SharedServiceMain::instance()->reconfigure( changes, AppConfig::instance());
}

