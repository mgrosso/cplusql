//
// client.cpp,v 1.4 2001/03/29 19:42:06 coryan Exp
//

// Include the generated names....
#include "JobQueueServiceC.h"
#include "AppLog.h"
#include "AppConfig.h"
#include "BaseException.h"
#include "ServiceStatusMap.h"
#include "SharedServiceMain.h"

extern char *optarg;
void usage()
{
    fprintf(stderr,"JobQueueClient [-pkamxcnlewh?]\n" );
    fprintf(stderr,"    --cfg=<file>                    configuration file <file>\n" );
    fprintf(stderr,"    -p or --GS_ping                 ping the JobQueueService\n" );
    fprintf(stderr,"    -k or --GS_kill                 kill all jobs\n" );
    fprintf(stderr,"    -a or --GS_cleanup_stale_output cleanup stale output on all nodes\n" );
    fprintf(stderr,"    -x or --JQS_exclusive           cwjobs sumitted are need exclusive JobExecs\n" );
    fprintf(stderr,"    -s <command>                    submit <command>\n");
    fprintf(stderr,"    -e <sleeptime>                  submit sleep <sleeptime>\n");
    fprintf(stderr,"    -c <submit_count>               how many items to submit\n");
    fprintf(stderr,"    -m <name>                       hostname <name>\n");
    fprintf(stderr,"    --GS_hostname=<name>            hostname <name>\n" );
    fprintf(stderr,"    -n <name>                       service name <name>\n");
    fprintf(stderr,"    --GS_service_name=<name>        service name <name>\n" );
    fprintf(stderr,"    --JQS_test_push_status_list     for test only\n");
    fprintf(stderr,"    -l or --GS_pull_status          pull status from the JobQueueService\n" );
    fprintf(stderr,"    -w or --JQS_WAIT_FOR_STATUS     pull status from the JobQueueService\n" );
    fprintf(stderr,"    -h or -? or anything else generates this message\n" );
    exit(1);
};

void ping(JobQueue::JobQueueService_var jobQueueService)
{
    APPLOG_DBG("calling ping");
    jobQueueService->ping();
    APPLOG_DBG("returned from ping");
}

void killAllJobs(JobQueue::JobQueueService_var jobQueueService, int numThreads)
{
    APPLOG_DBG("calling killAllJobs");
    jobQueueService->killAllJobs(numThreads);
    APPLOG_DBG("returned from killAllJobs");
}

void cleanupStaleOutput(JobQueue::JobQueueService_var jobQueueService, int numThreads)
{
    APPLOG_DBG("calling cleanupStaleOutput");
    jobQueueService->cleanupStaleOutput(numThreads);
    APPLOG_DBG("returned from cleanupStaleOutput");
}

int main (int argc, char* argv[])
{
    try 
    {
        AppConfig* appConfig = AppConfig::instance();

        appConfig->init(argc, argv);
        appConfig->consumeArgs(&argc, argv, true);
        appConfig->addString( "APPLOG_MINLOG", "0", false );
        AppLog::instance()->reconfigure( *appConfig );

        char arg;
        APPLOG_DBG("argc=%d, argv[0]=%s", argc, (argc>0?argv[0]:"NULL"));
        while(-1!=(arg=getopt( argc,argv,"pkae:xn:m:ls:c:tw:h?")))
        {
            APPLOG_DBG("arg=%c, optarg=%s", arg, (optarg==NULL?"NULL":optarg));
            switch (arg)
            {
                case 'p':
                    appConfig->addString("GS_ping","",true);
                    break;
                case 'k':
                    appConfig->addString("GS_kill","20",true);
                    break;
                case 'a':
                    appConfig->addString("GS_cleanup_stale_output","20",true);
                    break;
                case 'x':
                    appConfig->addString("JQS_exclusive","",true);
                    break;
                case 'w':
                    appConfig->addString("JQS_WAIT_FOR_STATUS",optarg,true);
                    break;
                case 'c':
                    appConfig->addString("JQS_submit_count",optarg,true);
                    break;
                case 'e':
                    appConfig->addString("JQS_sleep",optarg,true);
                    break;
                case 's':
                    appConfig->addString("JQS_submit",optarg,true);
                    break;
                case 'n':
                    appConfig->addString("GS_service_name",optarg,true);
                    break;
                case 'm':
                    appConfig->addString("GS_hostname",optarg,true);
                    break;
                case 'l':
                    appConfig->addString("GS_pull_status","",true);
                    break;
                case 't':
                    appConfig->addString("GS_pull_status_list","",true);
                    break;
                case 'h':
                case '?':
                default:
                    usage();
                    break;
            }
        }
        const char* url=appConfig->getString("GS_URL");
        APPLOG_DBG("url=%s", url);
    
        // First initialize the ORB, that will remove some arguments...
        APPLOG_DBG("init orb");
        SharedServiceMain::init( argc, argv, __FILE__, false );
        APPLOG_DBG("find GS_URL");
        const char * jqs_url = AppConfig::instance()->getString( "GS_URL" );
        APPLOG_DBG("doing resolv(%s)", jqs_url);
        CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve( jqs_url );
        if(obj == NULL)
        {
            PANICV("Could not create object from string");
            exit(1); // not reached
        }
        APPLOG_DBG("doing narrow()...");
        JobQueue::JobQueueService_ptr jobQueueService = JobQueue::JobQueueService::_narrow( obj );
        APPLOG_DBG("back from narrow()...");
        
        if (appConfig->findString("GS_cleanup_stale_output")) {
            int numThreads=30;
            if (appConfig->findString("JQS_submit_count"))
            {
                numThreads=appConfig->getInt("JQS_submit_count");
                if (numThreads < 0)
                {
                    PANICV("numThreads must be greater than or equal to zero");
                }
            }
            cleanupStaleOutput (jobQueueService, numThreads);
        } else if (appConfig->findString("GS_kill")) {
            int numThreads=30;
            if (appConfig->findString("JQS_submit_count"))
            {
                numThreads=appConfig->getInt("JQS_submit_count");
                if (numThreads < 0)
                {
                    PANICV("numThreads must be greater than or equal to zero");
                }
            }
            killAllJobs (jobQueueService, numThreads);
        } else if (appConfig->findString("GS_ping")) {
            ping (jobQueueService);
        } else if (appConfig->findString("GS_pull_status")) {
            APPLOG_DBG("calling jobQueueService->pullStatus");
            ServiceStatusPtr pServiceStatus = jobQueueService->pullStatus();
            APPLOG_DBG("back from pullStatus");
            ServiceStatusMap::printServiceStatus(pServiceStatus.get());
        } else if (const char* jobId = appConfig->findString("JQS_WAIT_FOR_STATUS")) {
            //saved from before, to be better placed soon
            APPLOG_DBG("JQS_WAIT_FOR_STATUS");
            APPLOG_DBG("jobId=%s",jobId);

            int timeout=30;
            if (appConfig->findString("JQS_submit_count"))
            {
                timeout=appConfig->getInt("JQS_submit_count");
                if (timeout < 0)
                {
                    PANICV("Error timeout must be greater than or equal to zero");
                }
            }
            JobQueue::JobIdList jobIdList(1);
            APPLOG_DBG("int length = jobIdList.length();");
            jobIdList.length(1);
            int length = jobIdList.length();
            APPLOG_DBG("length=%d",length);
            jobIdList[0] = jobId;
            for (int i=0; i<length; i++) 
            {
                APPLOG_DBG("in loop");
                APPLOG_DBG("jobIdList[%d]=%s", i, jobIdList[i].in());
            }
            
            APPLOG_DBG( "jobIdList.length()=%d", jobIdList.length());

            APPLOG_DBG("calling Service_JobQueueServiceImpl::waitForStatusList ()");
                
            JobQueue::JobStateIdList* jobStateIdList = 
                jobQueueService->waitForStatusList(jobIdList, timeout);
            APPLOG_DBG("returned from waitForStatusList");

            length = jobStateIdList->length();
            APPLOG_DBG("in JobQueueServiceImpl::submit: jobStateIdList->length()=%d", length);
            for (int i=0; i<length; i++) 
            {
                APPLOG_DBG("(*jobStateIdList)[%d]=%d", i, (*jobStateIdList)[i]);
            }
        } else if (const char* command = appConfig->findString("JQS_sleep")) {
            //saved from before, to be better placed soon
            APPLOG_DBG("JQS_sleep");
            int sleeptime = appConfig->getInt("JQS_sleep");
            char sleepCommand[100];
            sprintf(sleepCommand, "sleep %d", sleeptime);
            int submitCount=1;
            if (appConfig->findString("JQS_submit_count"))
            {
                submitCount=appConfig->getInt("JQS_submit_count");
                if (submitCount < 1)
                {
                    PANICV("Error submit count must be greter than zero");
                }
            }
            JobQueue::exec_args execArgs[submitCount];

            int exclusive=0;
            if (appConfig->findString("JQS_exclusive"))
            {
                exclusive=1;
            }
            for (int i=0; i<submitCount; i++)
            {
                execArgs[i].command=(const char*)sleepCommand;// will be copied
                execArgs[i].exclusive=exclusive;
            }

            JobQueue::ExecArgsList execArgsList(submitCount,submitCount,execArgs);
            
            APPLOG_DBG( "execArgsList.length()=%d", execArgsList.length());
            for (int i=0; i<submitCount; i++)
            {
                APPLOG_DBG("execArgsList[%d].command.in()=%s", i, execArgsList[i].command.in());
            }

            APPLOG_DBG("calling Service_JobQueueServiceImpl::submit ()");
                
            JobQueue::JobIdList* jobIdList = 
                jobQueueService->submit(execArgsList);
            APPLOG_DBG("returned from submit");

            int length = jobIdList->length();
            APPLOG_DBG("in JobQueueServiceImpl::submit: jobIdList->length()=%d", length);
            for (int i=0; i<length; i++) 
            {
                APPLOG_DBG("(*jobIdList)[%d]=%s", i, (*jobIdList)[i].in());
            }
        } else if (const char* command = appConfig->findString("JQS_submit")) {
            //saved from before, to be better placed soon
            APPLOG_DBG("JQS_submit");
            int submitCount=1;
            if (appConfig->findString("JQS_submit_count"))
            {
                submitCount=appConfig->getInt("JQS_submit_count");
                if (submitCount < 1)
                {
                    PANICV("Error submit count must be greter than zero");
                }
            }
            JobQueue::exec_args execArgs[submitCount];

            int exclusive=0;
            if (appConfig->findString("JQS_exclusive"))
            {
                exclusive=1;
            }
            for (int i=0; i<submitCount; i++)
            {
                execArgs[i].command=command;// will be copied
                execArgs[i].exclusive=exclusive;
            }

            JobQueue::ExecArgsList execArgsList(submitCount,submitCount,execArgs);
            
            APPLOG_DBG( "execArgsList.length()=%d", execArgsList.length());
            for (int i=0; i<submitCount; i++)
            {
                APPLOG_DBG("execArgsList[%d].command.in()=%s", i, execArgsList[i].command.in());
            }

            APPLOG_DBG("calling Service_JobQueueServiceImpl::submit ()");
                
            JobQueue::JobIdList* jobIdList = 
                jobQueueService->submit(execArgsList);
            APPLOG_DBG("returned from submit");

            int length = jobIdList->length();
            APPLOG_DBG("in JobQueueServiceImpl::submit: jobIdList->length()=%d", length);
            for (int i=0; i<length; i++) 
            {
                APPLOG_DBG("(*jobIdList)[%d]=%s", i, (*jobIdList)[i].in());
            }
        } else if (appConfig->findString("JQS_test_push_status_list")) {

            Service::NVPair nvPair[3];
            nvPair[0].name="JE_used_slots";
            nvPair[0].value="0";
            nvPair[1].name="JE_max_slots";
            nvPair[1].value="2";
            nvPair[2].name="JE_available_slots";
            nvPair[2].value="2";
            
            Service::NVPairList details(3, 3, nvPair);

            //Service::ServiceStatus serviceStatus[2];
            Service::ServiceStatus serviceStatus[1];
            serviceStatus[0].ref.serviceName="JobExecService";
            serviceStatus[0].ref.url="corbaname::sf-devdw020:12000#sf-devdw020_0_JobExecService";
            serviceStatus[0].ref.hostname="sf-devdw020";
            serviceStatus[0].pid=11111;
            serviceStatus[0].isPendingShutdown=false;
            serviceStatus[0].live=true;
            serviceStatus[0].details = details;

            Service::ServiceStatusList serviceStatusList(1,1,serviceStatus);
            
            Service::ServiceStatus serviceStatusParam=serviceStatusList[0];
            APPLOG_DBG("calling Service_NodeMonitorPushConsumerImpl::pushStatus ( %s )" , serviceStatusParam.ref.url.in());
            serviceStatusParam=serviceStatusList[0];
            APPLOG_DBG( "calling JobQueueServiceImpl::pushStatus ( %s )", serviceStatusParam.ref.url.in());
                
            jobQueueService->pushStatusList(serviceStatusList);
            APPLOG_DBG("returned from pushStatus");
        }
        
        SharedServiceMain::instance()->getORB()->destroy ();
        APPLOG_DBG("orb destroyed");
    }
    catch (CORBA::Exception &ce) {
        APPLOG_ABORT("JobQueueClient main(): caught CORBA::Exception [%s][%s]",
            ce._rep_id(), ce._name() );
        return 1;
    }
    catch (ConfigNoSuchKey &e) {
        fprintf(stderr, "missing parameter: %s\n", e.what());
        usage();
        return 1;
    }
    catch( exception &e )
    {
        fprintf(stderr, "caught exception in main: %s\n", e.what());
        return 1;
    }
    return 0;
}
