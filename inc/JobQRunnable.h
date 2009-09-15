
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      JobQRunnable.h
// Author:    mgrosso 
// Created:   Wed Sep  8 18:19:37 EDT 2004 on caliban
// Project:   
// Purpose:   submit and track status of multiple jobs using only one 
//            thread and one corba object.
// 
// $Id: JobQRunnable.h,v 1.3 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef JOBQRUNNABLE_H
#define JOBQRUNNABLE_H 1

#include "ptr_strdup.h"
#include "Config.h"
#include "Runnable.h"

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#include "JobQueueServiceC.h"
#include "serviceC.h"
#endif
#endif

#include <sys/time.h>

class JobQRunnablePriv ;
class JobQRunnable : public Runnable
{
    public:
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
                                    JobQRunnable( 
                                        JobQueue::JobQueueService_ptr jobq );
    JobQueue::JobQueueService_ptr   getJobQ();
#endif
#endif
    virtual                         ~JobQRunnable();


    void                            addJob( 
        const char *jobname, const char *cmd, ConfigPtr &cfg);

    virtual void                    run();
    private:
    JobQRunnablePriv *d_;
    //not implemented
    JobQRunnable();
    JobQRunnable( const JobQRunnable &rhs );
    JobQRunnable &operator=( const JobQRunnable &rhs );
};

typedef ptr< JobQRunnable > JobQRunnablePtr ;

#endif /* JOBQRUNNABLE_H */
