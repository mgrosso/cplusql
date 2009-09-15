
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      JobRunnable.h
// Author:    sjackson Steve Jackson
// Created:   Wed Mar 17 14:14:40 PST 2004 on localhost.localdomain
// Project:
// Purpose:
//
// Copyright (c) 2004 LookSmart. All Rights Reserved.
//
// $Id: JobRunnable.h,v 1.7 2004/04/18 03:43:16 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#ifndef JOBRUNNABLE_H_
#define JOBRUNNABLE_H_

#include "ptr_strdup.h"
#include "Runnable.h"
#include "JobQueueServiceC.h"
#include "serviceC.h"
#include <sys/time.h>

class Config;

class JobRunnable : public Runnable
{
public:

	JobRunnable(const char_ptr &jobname, const char_ptr &cmd, JobQueue::JobQueueService_ptr jobq, ConfigPtr cfg);
	virtual ~JobRunnable();
	virtual void run();

protected:
	enum WhatToDo { waitLonger, succeeded = JobQueue::succeeded , failed = JobQueue::failed };

	WhatToDo checkStatus(const char *id);

	char_ptr messageShutdown(Service::PendingShutdownException &err, const char *msg, const char *jobname );
	char_ptr messageServiceException(Service::ServiceException &err, const char *msg, const char *jobname );
	char_ptr messageCorbaException(CORBA::Exception &err, const char *msg, const char *jobname );

	void setupEnv(JobQueue::exec_args &);

	void setupConfig(JobQueue::exec_args &);
	void addConfigIf( Service::NVPairList &cfg, Config &c, const char *key );

	// Returns the state of this job.
	//
	JobQueue::JobStateId getRunState() const;

	// Return the time of start, in ms.  If not started, returns 0L.
	//
	long getStartMillis() const;

	// Return the time of start, in ms.  If not ended, returns 0L.
	//
	long getEndMillis() const;

	// Return the time of start, in ms.  If not started, returns 0L.  If
	// not yet ended, returns current time - start time.
	//
	long getRuntimeMillis() const;

private:
	//not implemented
	JobRunnable( const JobRunnable &rhs );
	JobRunnable &operator=( const JobRunnable &rhs );

	// Convert the timeval to milliseconds.
	//
	long getMillis(const struct timeval &) const;

	// Get the current time, and log a warning on failure. Return true if oik.
	//
	static bool getTime(struct timeval &);

	mutable struct timeval startTime_;
	mutable struct timeval endTime_;
	JobQueue::JobStateId jobStatus_;

	Service::time_t timeout_;

	char_ptr jobName_;
	char_ptr cmd_;
	JobQueue::JobQueueService_ptr jobQStub_;
	ConfigPtr cfg_;
};
typedef ptr< JobRunnable > JobPtr ;

#endif /* JOBRUNNABLE_H_ */
