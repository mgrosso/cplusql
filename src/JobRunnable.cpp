
#include "ace/String_Base.h"
#include "Convert.h"
#include "AppLog.h"

#include "JobRunnable.h"
#include "keys.h"
#include "NVPairUtil.h"

using namespace JobQueue;

JobRunnable::JobRunnable(const char_ptr &jobname, const char_ptr &cmd, JobQueueService_ptr jobq, ConfigPtr cfg) : jobStatus_(JobQueue::ready), timeout_(0), jobName_(jobname), cmd_(cmd), jobQStub_(jobq), cfg_(cfg)
{
}

JobRunnable::~JobRunnable()
{
	if (jobQStub_ != (JobQueueService_ptr) 0)
	{
		// can't do this, as is protected: delete jobQStub_;
		jobQStub_ = (JobQueueService_ptr) 0;
	}
}


JobQueue::JobStateId
JobRunnable::getRunState() const
{
	return jobStatus_;
}


bool
JobRunnable::getTime(struct timeval &tv)
{
	bool retVal = true;

	// On Solaris the 2nd arg is a void *, but under linux a timezone *,
	// so to be portable we'll just use a bare nake 0.
	//
	if (gettimeofday(&tv, 0) == -1)
	{
		APPLOG_WARN("Failed to gettimeofday %d", errno);
		retVal = false;
	}

	return retVal;
}


long
JobRunnable::getMillis(const struct timeval &tv) const
{
	long retVal = 0L;

	if (tv.tv_sec > 0L)
	{
		long ms = (tv.tv_usec / 10) % 1000;
		long s = tv.tv_sec * 1000 + ms;

		retVal = s + ms;
	}

	return retVal;
}


long
JobRunnable::getStartMillis() const
{
	return getMillis(startTime_);
}


long
JobRunnable::getEndMillis() const
{
	return getMillis(endTime_);
}


long
JobRunnable::getRuntimeMillis() const
{
	long retVal = 0L;
	long startMillis = getStartMillis();

	if (startMillis > 0L)
	{
		long endMillis = getEndMillis();

		if (endMillis == 0L)
		{
			struct timeval now;

			if (!getTime(now))
			{
				return retVal;
			}

			endMillis = getMillis(now);
		}

		retVal = endMillis - startMillis;
	}

	return retVal;
}


void
JobRunnable::run()
{
	getTime(startTime_);
    char_ptr errmsg;
	if (jobQStub_ == (JobQueueService_ptr) 0)
	{
        PANICV("cant submit job (%s) because jobQstub is null!", jobName_.get());
    }
    const char * id;
    ExecArgsList eal;
    exec_args cmd;
    JobIdList *ids ;

    APPLOG_INFO("Running %s", jobName_.get());

    cmd.command = CORBA::string_dup(cmd_.get());
    setupEnv(cmd);
    setupConfig(cmd);

    eal.length(1);
    eal._allocate_buffer(1);
    eal[0] = cmd;

    try {
        APPLOG_DBG("%s: submitting %s", jobName_.get(), cmd_.get());

        jobStatus_ = JobQueue::submitting;
        ids = jobQStub_->submit(eal);

        if ((ids != (JobIdList *) 0) && (ids->length() > 0))
        {
            id = ((*ids)[0]).in();
            APPLOG_DBG("%s: got ID of %s", jobName_.get(), id);
        }
        if( id == NULL )
        {
            errmsg = ptr_strdup_plus(   "Failed to get a jobId for ", jobName_.get());
        }
    } catch ( Service::PendingShutdownException &pse ) {
        errmsg = messageShutdown( pse, "cant submit", jobName_.get());
    } catch ( Service::ServiceException &se ) {
        errmsg = messageServiceException( se, "cant submit", jobName_.get());
    } catch ( CORBA::Exception &ce ) {
        errmsg = messageCorbaException( ce, "cant submit", jobName_.get());
    }
    if( errmsg.get() != NULL )
    {
        PANICV( errmsg.get());
    }

    APPLOG_DBG("%s: after submit of %s", jobName_.get(), cmd_.get());

    try {
        WhatToDo more;

        do {
            APPLOG_DBG("%s: waiting for %ld", jobName_.get(), timeout_);
            jobStatus_ = jobQStub_->waitForStatus(id, timeout_);
            more = checkStatus(id);
        } while (more == waitLonger);
    } catch ( Service::PendingShutdownException &pse ) {
        errmsg = messageShutdown( pse, "cant get status", jobName_.get());
    } catch ( Service::ServiceException &se ) {
        errmsg = messageServiceException( se, "cant get status", jobName_.get());
    } catch ( CORBA::Exception &ce ) {
        errmsg = messageCorbaException( ce, "cant get status", jobName_.get());
    }
    if( errmsg.get() != NULL )
    {
        PANICV( errmsg.get());
    }

    getTime(endTime_);

    APPLOG_DBG("%s: status=%d, runtime=%ld", jobName_.get(), (int) jobStatus_,
        getRuntimeMillis());
    if( jobStatus_ == JobQueue::failed )
    {
        PANICV("job failed: %s runtime=%ld", jobName_.get(), getRuntimeMillis());
    }
}


JobRunnable::WhatToDo
JobRunnable::checkStatus(const char *id)
{
	WhatToDo retVal = succeeded;

	APPLOG_INFO("%s: jid=%s, state=%d", jobName_.get(), id, (int) jobStatus_);

	if (jobStatus_ == JobQueue::failed)
	{
		retVal = JobRunnable::failed;
	}
	else if (jobStatus_ != JobQueue::succeeded)
	{
		retVal = JobRunnable::waitLonger;
	}

	return retVal;
}


char_ptr
JobRunnable::messageShutdown(Service::PendingShutdownException &except, const char *msg, const char *jobname )
{
    char errbuf[1024];
    jobStatus_ = JobQueue::failed;
    errbuf[0]='\0';
    errbuf[1023]='\0';
    snprintf( errbuf, 1023, "problem with job [%s] %s because server shutdown is pending ( %s, %s )",
        jobname, msg, except._rep_id(), except._name()
    );
    return ptr_strdup( errbuf);
}

char_ptr
JobRunnable::messageServiceException(Service::ServiceException &except, const char *msg, const char *jobname )
{
    char errbuf[1024];
    jobStatus_ = JobQueue::failed;
    const char *what_string = except.what_string ;
    errbuf[0]='\0';
    errbuf[1023]='\0';
    snprintf( errbuf, 1023, "problem with job [%s] %s because received service exception ( %s, %s ): %i %s ",
        jobname, msg, except._rep_id(), except._name(), except.errno_short, what_string
    );
    return ptr_strdup( errbuf);
}

char_ptr
JobRunnable::messageCorbaException(CORBA::Exception &except, const char *msg, const char *jobname )
{
    char errbuf[1024];
    jobStatus_ = JobQueue::failed;
    errbuf[0]='\0';
    errbuf[1023]='\0';
    snprintf( errbuf, 1023, "problem with job [%s] %s because received corba exception ( %s, %s )",
        jobname, msg, except._rep_id(), except._name()
    );
    return ptr_strdup( errbuf);
}

void
JobRunnable::addConfigIf( Service::NVPairList &cfg, Config &c, const char *key )
{
    if( c.findString( key ))
    {
        NVPairUtil::append( &cfg, key, cfg_->getString(key));
    }
}

void
JobRunnable::setupConfig( exec_args &cmd )
{
    Service::NVPairList &jconf =cmd.configuration;

    timeout_ = cfg_->getInt(JOB_EXCLUSIVE, "0" );
    cmd.exclusive = cfg_->getBool(JOB_TIMEOUT, "0" );

    char cwdbuf[1024];
    memset( cwdbuf, '\0', 1024 );
    if( NULL== getcwd( cwdbuf, 1023 ))
    {
        PANICV( "could not determind current directory.");
    }
    NVPairUtil::append( 
        &jconf, JOB_STARTDIR, cfg_->getString( JOB_STARTDIR, cwdbuf ));

    addConfigIf( jconf, *cfg_, JOB_ERROR );
    addConfigIf( jconf, *cfg_, JOB_OUTPUT );
    addConfigIf( jconf, *cfg_, JOB_TARGET );
    addConfigIf( jconf, *cfg_, JOB_REQUIRES_HOST );
}

void
JobRunnable::setupEnv(exec_args &cmd)
{
	EnvPairList &env = cmd.environment;
#if 0
	char_ptr_vec *keys = cfg_.get()->getKeys();
	int at = 0;

	env.length(keys->size());
	env._allocate_buffer(keys->size());

	for (size_t i = 0; i < keys->size(); i++)
	{
		const char *key = (*keys)[i].get();
		const char *val = cfg_.get()->getString(key);

		try {
			if (strcmp(key, JOB_EXCLUSIVE) == 0) {
				bool b = Convert::convertToBool(val);

				APPLOG_DBG("job %s will %srun exclusive: %d", b ? "" : "not ");

				cmd.exclusive = b;
			}
			else if (strcmp(key, JOB_TIMEOUT) == 0)
			{
				timeout_ = Convert::convertToLong(val);
			}
			else
			{
				EnvPair ep;

				APPLOG_DBG("job %s env %s=%s", jobName_.get(), key, val);

				ep.name = key;
				ep.value = val;

				env[at++] = ep;
			}
		} catch ( ConfigBadConversion &cbc ) {
			APPLOG_ALARM("bad conversion of %s=%s", key, val);
		}
	}

	delete keys;
#endif
}

