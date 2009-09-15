//
// client.cpp,v 1.4 2001/03/29 19:42:06 coryan Exp
//

// Include the generated names....
#include "serviceC.h"
#include "AppLog.h"
#include "AppConfig.h"
#include "BaseException.h"
#include "ServiceStatusMap.h"
#include "keys.h"

extern char *optarg;
void usage()
{
    fprintf(stderr,"NodeMonitorClient [-pnrusxlth?]\n" );
    fprintf(stderr,"    --cfg=<file>                    configuration file <file>\n" );
    fprintf(stderr,"    -p or --GS_ping                 ping the NodeMonitorService\n" );
    fprintf(stderr,"    -m <name>                       hostname <name>\n");
    fprintf(stderr,"    --GS_hostname=<name>            hostname <name>\n" );
    fprintf(stderr,"    -n <name>                       service name <name>\n");
    fprintf(stderr,"    --GS_service_name=<name>        service name <name>\n" );
    fprintf(stderr,"    -r <registrant>                 register service <registrant_url>\n");
    fprintf(stderr,"    --NMS_register=<registrant>     register service <registrant_url>\n" );
    fprintf(stderr,"    -u <registrant>                 unregister service <registrant_url>\n");
    fprintf(stderr,"    --NMS_unregister=<registrant>   unregister service <registrant_url>\n" );
    fprintf(stderr,"    -s <subscriber>                 subscribe service <subscriber>\n");
    fprintf(stderr,"    --NMS_subscribe=<subscriber>    subscribe service <subscriber>\n" );
    fprintf(stderr,"    -x <subscriber>                 unsubscribe service <subscriber>\n");
    fprintf(stderr,"    --NMS_unsubscribe=<subscriber>  unsubscribe service <subscriber>\n" );
    fprintf(stderr,"    --NMS_test_push_status_list     for test only");
    fprintf(stderr,"    -l or --GS_pull_status          pull status from the NodeMonitorService\n" );
    fprintf(stderr,"    -t or --GS_pull_status_list     pull status list from the NodeMonitorService\n" );
    fprintf(stderr,"    -h or -? or anything else generates this message\n" );
    exit(1);
};

void ping(Service::NodeMonitorService_var nodeMonitorService)
{
    APPLOG_DBG("calling ping");
    nodeMonitorService->ping();
    APPLOG_DBG("returned from ping");
}

int main (int argc, char* argv[])
{
    try 
    {
        AppConfig* appConfig = AppConfig::instance();
        
        appConfig->init(argc, argv);
        appConfig->consumeArgs(&argc, argv);
        appConfig->addString( "APPLOG_MINLOG", "0", false );
        AppLog::instance()->reconfigure( *appConfig );

        char arg;
        APPLOG_DBG("getopt");
        while(-1!=(arg=getopt( argc,argv,"pn:m:r:u:s:x:lth?")))
        {
            switch (arg)
            {
                case 'p':
                    appConfig->addString("GS_ping","",true);
                    break;
                case 'n':
                    appConfig->addString("GS_service_name",optarg,true);
                    break;
                case 'm':
                    appConfig->addString("GS_hostname",optarg,true);
                    break;
                case 'r':
                    if( optarg )
                    {
                        appConfig->addString("NMS_register",optarg,true);
                    }
                    break;
                case 'u':
                    if( optarg )
                    {
                        appConfig->addString("NMS_unregister",optarg,true);
                    }
                    break;
                case 's':
                    if( optarg )
                    {
                        appConfig->addString("NMS_subscribe",optarg,true);
                    }
                    break;
                case 'x':
                    if( optarg )
                    {
                        appConfig->addString("NMS_unsubscribe",optarg,true);
                    }
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
    
        // First initialize the ORB, that will remove some arguments...
        CORBA::ORB_var orb =
            CORBA::ORB_init (argc, argv,
                             "" /* the ORB name, it can be anything! */);

        CORBA::Object_ptr nodeMonitorService_object =
            orb->string_to_object (url);

        if (CORBA::is_nil(nodeMonitorService_object)) {
            PANICV("Could not create object from string %s", url);
        }

        // Now downcast the object reference to the appropriate type
        Service::NodeMonitorService_var nodeMonitorService =
        Service::NodeMonitorService::_narrow (nodeMonitorService_object);

        if (CORBA::is_nil (nodeMonitorService.in ()))
        {
            PANICV("ERROR: Object reference <%s> is nil", url);
        }

        if (appConfig->findString("GS_ping")) {
            ping (nodeMonitorService);

        } else if (const char* registerUrl = appConfig->findString("NMS_register")) {
            Service::ServiceRef serviceRef;
            serviceRef.url=registerUrl;
            serviceRef.serviceName=appConfig->getString("GS_service_name");

            APPLOG_DBG("serviceRef_var.url=%s", serviceRef.url.in());
            APPLOG_DBG("serviceRef.ServiceName=%s", serviceRef.serviceName.in());
            APPLOG_DBG("calling registerService");
            nodeMonitorService->registerService(serviceRef);
            APPLOG_DBG("back from registerService");

        } else if (const char* unregisterUrl = appConfig->findString("NMS_unregister")) {
            Service::ServiceRef serviceRef;
            serviceRef.url=unregisterUrl;
            serviceRef.serviceName=appConfig->getString("GS_service_name");

            APPLOG_DBG("serviceRef_var.url=%s", serviceRef.url.in());
            APPLOG_DBG("serviceRef.ServiceName=%s", serviceRef.serviceName.in());
            APPLOG_DBG("calling unregisterService");
            nodeMonitorService->unregisterService(serviceRef);
            APPLOG_DBG("back from unregisterService");

        } else if (const char* subscriberUrl = appConfig->findString("NMS_subscribe")) {
            Service::ServiceRef serviceRef;
            serviceRef.url=subscriberUrl;
            serviceRef.serviceName=appConfig->getString("GS_service_name");

            APPLOG_DBG("serviceRef_var.url=%s", serviceRef.url.in());
            APPLOG_DBG("serviceRef.ServiceName=%s", serviceRef.serviceName.in());
            APPLOG_DBG("calling subscribePushConsumer");
            nodeMonitorService->subscribePushConsumer(serviceRef, "*", "*");
            APPLOG_DBG("back from subscribePushConsumer");
        } else if (const char* registerUrl = appConfig->findString("NMS_unsubscribe")) {
            Service::ServiceRef serviceRef;
            serviceRef.url=registerUrl;
            serviceRef.serviceName=appConfig->getString("GS_service_name");

            APPLOG_DBG("serviceRef_var.url=%s", serviceRef.url.in());
            APPLOG_DBG("serviceRef.ServiceName=%s", serviceRef.serviceName.in());
            APPLOG_DBG("calling subscribePushConsumer");
            nodeMonitorService->unsubscribePushConsumer(serviceRef);
            APPLOG_DBG("back from register");
        } else if (appConfig->findString("GS_pull_status_list")) {
            APPLOG_DBG("calling nodeMonitorService->pullStatusList");

            const char* hostname=appConfig->getString("GS_hostname", "*");
            const char* serviceName=appConfig->getString("GS_service_name", "*");

            Service::ServiceStatusList* serviceStatusList = nodeMonitorService->pullStatusList(serviceName, hostname);
            ptr<Service::ServiceStatusList> pServiceStatusList = serviceStatusList;

            APPLOG_DBG("back from pullStatus");
            int length = serviceStatusList->length();

            APPLOG_DBG("in pullStatusList: serviceStatusList->length()=%d", length);
                    
            for (int i=0; i<length; i++) {
                
                ServiceStatusMap::printServiceStatus(&(*serviceStatusList)[i]);
            }
        } else if (appConfig->findString("GS_pull_status")) {
            APPLOG_DBG("calling nodeMonitorService->pullStatus");
            ServiceStatusPtr pServiceStatus = nodeMonitorService->pullStatus();
            APPLOG_DBG("back from pullStatus");
            ServiceStatusMap::printServiceStatus(pServiceStatus.get());
        } else if (appConfig->findString("NMS_test_push_status_list")) {
            //saved from before, to be better placed soon
            Service::ServiceStatus serviceStatus[2];
            serviceStatus[0].ref.serviceName=JE_NAME ;
            serviceStatus[0].ref.url="testUrl1";
            serviceStatus[0].ref.hostname="testhost1";
            serviceStatus[0].pid=11111;
            serviceStatus[0].isPendingShutdown=false;
            serviceStatus[0].live=true;

            serviceStatus[1].ref.serviceName="NodeMonitor";
            serviceStatus[1].ref.url="testUrl2";
            serviceStatus[1].ref.hostname="testhost2";
            serviceStatus[1].pid=22222;
            serviceStatus[1].isPendingShutdown=false;
            serviceStatus[1].live=true;

//            Service::ServiceStatusList serviceStatusList(2,2,serviceStatus);
            Service::ServiceStatusList serviceStatusList(2);
            
            Service::ServiceStatus serviceStatusParam=serviceStatusList[0];
            APPLOG_DBG("calling Service_NodeMonitorPushConsumerImpl::pushStatus ( %s )" , serviceStatusParam.ref.url.in());
            serviceStatusParam=serviceStatusList[1];
            APPLOG_DBG( "calling Service_NodeMonitorPushConsumerImpl::pushStatus ( %s )", serviceStatusParam.ref.url.in());
            APPLOG_DBG( "serviceStatusList.length()=%d", serviceStatusList.length());
                
            nodeMonitorService->pushStatusList(serviceStatusList);
            APPLOG_DBG("returned from pushStatus");
        }
        // Finally destroy the ORB
       
        orb->destroy ();
        APPLOG_DBG("orb destroyed");
        return 0;
    }
    catch (CORBA::Exception &e) {
        APPLOG_WARN("CORBA exception raised!");
    }
    catch (ConfigNoSuchKey &ce) {
        usage();
    }
    catch (std::exception &e) {
        APPLOG_WARN("exception caught: %s", e.what());
    }
    return 1;
}
