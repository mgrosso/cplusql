#ident "file_id $Id: JobQRunnable.cpp,v 1.4 2005/10/17 11:18:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      JobQRunnable.cpp
// Author:    mgrosso 
// Created:   Wed Sep  8 18:19:37 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: JobQRunnable.cpp,v 1.4 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "cplusql.h"
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#include "JobQRunnable.h"

#include "ace/String_Base.h"
#include "Convert.h"
#include "AppLog.h"

#include "JobRunnable.h"
#include "keys.h"
#include "NVPairUtil.h"

using namespace JobQueue;

class JobQRunnablePriv 
{
    private:
    friend class JobQRunnable ;
    list < exec_args * >            listea_ ;
    JobQueue::JobQueueService_ptr   jobq_   ;

    JobQRunnablePriv(
        JobQueue::JobQueueService_ptr   &jobq
    )
    :jobq_(jobq)
    {
        ;//noop
    };

    void addConfigIf( Service::NVPairList &cfg, Config &c, const char *key )
    {
        if( c.findString( key ))
        {
            NVPairUtil::append( &cfg, key, c.getString(key));
        }
    };

    char_ptr
    messageShutdown(Service::PendingShutdownException &except, const char *msg)
    {
        char errbuf[1024];
        errbuf[0]='\0';
        errbuf[1023]='\0';
        snprintf( errbuf, 1023, "%s server shutdown is pending ( %s, %s )",
            msg, except._rep_id(), except._name()
        );
        return ptr_strdup( errbuf);
    };

    char_ptr
    messageServiceException(
        Service::ServiceException &except, 
        const char *msg )
    {
        char errbuf[1024];
        const char *what_string = except.what_string ;
        errbuf[0]='\0';
        errbuf[1023]='\0';
        snprintf( errbuf, 1023, "%s, service exception ( %s, %s ): %i %s ",
            msg, except._rep_id(), except._name(), except.errno_short, what_string
        );
        return ptr_strdup( errbuf);
    };

    char_ptr
    messageCorbaException(
        CORBA::Exception &except, const char *msg )
    {
        char errbuf[1024];
        errbuf[0]='\0';
        errbuf[1023]='\0';
        snprintf( errbuf, 1023, "%s, corba exception ( %s, %s )",
            msg, except._rep_id(), except._name()
        );
        return ptr_strdup( errbuf);
    };
};

JobQRunnable::JobQRunnable( JobQueue::JobQueueService_ptr jobq )
{
    d_=new JobQRunnablePriv (jobq);
}

JobQRunnable::~JobQRunnable()
{
    delete d_;
}


void
JobQRunnable::addJob(
    const char *jobname, const char *command, ConfigPtr &cfg)
{
    exec_args *cmd = new exec_args ;

    cmd->command = CORBA::string_dup(command);
    cmd->exclusive = cfg->getBool(JOB_TIMEOUT, "0" );

    char cwdbuf[1024];
    memset( cwdbuf, '\0', 1024 );
    if( NULL== getcwd( cwdbuf, 1023 ))
    {
        PANICV( "could not determind current directory.");
    }
    NVPairUtil::append(
        &cmd->configuration, 
        JOB_STARTDIR, 
        cfg->getString( JOB_STARTDIR, cwdbuf ));

    d_->addConfigIf( cmd->configuration, *cfg, JOB_ERROR );
    d_->addConfigIf( cmd->configuration, *cfg, JOB_OUTPUT );
    d_->addConfigIf( cmd->configuration, *cfg, JOB_TARGET );
    d_->addConfigIf( cmd->configuration, *cfg, JOB_REQUIRES_HOST );

    NVPairUtil::append(
        &cmd->configuration, 
        "CPLUSQL_JOBNAME", 
        jobname );
    d_->listea_.push_back(cmd);
}

void
JobQRunnable::run()
{
    JobIdList *ids ;
    ExecArgsList eal    ;
    char_ptr errmsg;
    if( d_->listea_.empty())
    {
        APPLOG_DBG("no jobs to submit" );
        return;
    }

    //deep copy the dynamic list into static array.
    size_t e = d_->listea_.size();
    eal.length(e);
    eal._allocate_buffer(e);
    for( size_t i=0; i< e ; ++i )
    {
        eal[i] = * d_->listea_.front();
        delete d_->listea_.front();
        d_->listea_.pop_front();
    }

    APPLOG_DBG("submitting %lu jobs", e );
    try {
        ids = d_->jobq_->submit( eal );
        if( ! ids )
        {
            PANICV( "not jobids returned from jobq");
        }
   //
    //catch the following and rethrow as Base::Exception so that upper
    //layers of callstack need not know about these types of exceptions.  we
    //allow std::exception and ... to pass through unhindered.
    //
    } catch ( Service::PendingShutdownException &pse ) {
        errmsg = d_->messageShutdown( pse, "cant submit" );
    } catch ( Service::ServiceException &se ) {
        errmsg = d_->messageServiceException( se, "cant submit" );
    } catch ( CORBA::Exception &ce ) {
        errmsg = d_->messageCorbaException( ce, "cant submit" );
    } 
    if( errmsg.get() != NULL )
    {
        PANICV( errmsg.get());
    }
    //its helpful for debugging that we allow user to relate our
    //user jobname to jobq jobid.
    for( size_t i = 0; i < ids->length(); ++i )
    {
        const char *jobid = (*ids)[i];
        ConfigPtr jobcfg = NVPairUtil::convert( eal[i].configuration );
        const char *jobname = jobcfg->getString( "CPLUSQL_JOBNAME" );
        APPLOG_INFO("submitted jobname %s is jobid %s",
            jobname, jobid );
    }
    JobIdList *nextidlist ;
    while( ids->length())
    {
        JobStateIdList *jsil ;
        try{
            size_t ii = 0 ;
            size_t ee = ids->length();
            for( ; ii != ee ; ++ii )
            {
                APPLOG_DBG("calling waitForStatusList() with job %i=%s ",
                    ii, (*ids)[ii].in());
            }
            APPLOG_DBG("calling waitForStatusList() with %i entries", ee );
            jsil = d_->jobq_->waitForStatusList( *ids, 0 );
            APPLOG_DBG("done calling waitForStatusList()" );
        } catch ( Service::PendingShutdownException &pse ) {
            errmsg = d_->messageShutdown( pse, "cant getstatus" );
        } catch ( Service::ServiceException &se ) {
            errmsg = d_->messageServiceException( se, "cant getstatus" );
        } catch ( CORBA::Exception &ce ) {
            errmsg = d_->messageCorbaException( ce, "cant getstatus" );
        } 
        if( errmsg.get() != NULL )
        {
            PANICV( errmsg.get());
        }
        size_t i=0;
        size_t remaining =0;
        size_t e = jsil->length(); 
        for( ; i< e; ++i )
        {
            if( (*jsil)[i] !=  JobQueue::failed && 
                (*jsil)[i] !=  JobQueue::succeeded)
            {
                ++remaining;
            }
        }
        APPLOG_DBG( "current status list is %lu, returned=%lu remaining=%lu", 
            ids->length(), jsil->length(), remaining );
        nextidlist = new JobIdList();
        nextidlist->length(remaining) ;
        nextidlist->_allocate_buffer(remaining) ;
        size_t j=0;
        for( i=0; i<e; ++i )
        {
            const char *jobid = (*ids)[i].in();
            JobStateId jstate = (*jsil)[i] ;
            switch( jstate )
            {
                case JobQueue::failed :
                    APPLOG_WARN( "failed: job %s index=%lu", jobid, i );
                    errmsg = errmsg + " failed[";
                    errmsg = errmsg + jobid ;
                    errmsg = errmsg + "]";
                    break;
                case JobQueue::succeeded:
                    APPLOG_INFO("succeeded job %s index=%lu", jobid, i );
                    break;
                default:
                    (*nextidlist)[j] = jobid ;
                    APPLOG_DBG( 
                        "job %s index=%lu new index=%lu state=%i",
                        jobid, i, j, static_cast<int>(jstate));
                    ++j;
            }
        }
        delete ids;
        ids = nextidlist;
    }
    delete nextidlist;
    if( errmsg.get())
    {
        PANICV(errmsg.get());
    }
    APPLOG_DBG("all jobs suceeded.");
}
#endif
#endif
