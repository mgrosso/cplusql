
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#include "AppLog.h"
#include "AppConfig.h"
#include "clusterioC.h"
#include "FileHandler.h"
#include "keys.h"
#include "NVPairUtil.h"
#include "serviceC.h"
#include "SharedServiceMain.h"



AppConfig *appConfig = (AppConfig *) 0;

#define DEFAULT_MAX_TIME 60
#define DEFAULT_USECOND_TIME 1000000



bool
opPing(Service::GenericService_ptr gs)
{
	bool retVal = false;
	try {
		APPLOG_DBG("calling ping");
		gs->ping();
		retVal = true;
		APPLOG_DBG("pinged");
	} catch (Service::PendingShutdownException &pse) {
		APPLOG_INFO("caught pending shutdown");
	} catch (Service::ServiceException &se) {
		APPLOG_INFO("caught serviceException %s\n", se.what_string.in());
	} catch (CORBA::SystemException &se) {
		APPLOG_INFO("caught CORBA::SystemException: %s\n", se._info().rep());
		se._tao_print_system_exception(stderr);
	} catch (exception &e) {
		APPLOG_INFO("caught exception in main: %s\n", e.what());
	} catch (...) {
		APPLOG_INFO("caught something\n");
	}

	return retVal;
}



void
printUsageAndExit(int retCode)
{
	(void) ::fprintf(stderr, "usage: -s <sleep micro Seconds> -u <url> -w <maxTime>\n");
	(void) ::fprintf(stderr, "url can be obtained from $GS_URL\n");
	(void) ::fprintf(stderr, "default sleep time (micro seconds): %ld\n", DEFAULT_USECOND_TIME);
	(void) ::fprintf(stderr, "default max wait time (seconds): %ld\n", DEFAULT_MAX_TIME);


	exit(retCode);
}


int main(int argc, char *argv[])
{
	int retVal = 1;
	unsigned long uTime = DEFAULT_USECOND_TIME;
	int maxTime = DEFAULT_MAX_TIME;
	const char *url = (const char *) 0;

	try {
		appConfig = AppConfig::instance();

		appConfig->init(argc, argv);
		appConfig->consumeArgs(&argc, argv);
		appConfig->addString( "APPLOG_MINLOG", "0", false );
		AppLog::instance()->reconfigure( *appConfig );

		int arg;

		while ((arg = ::getopt(argc, argv, "s:u:w:")) != -1) {
			switch (arg) {
			case 's':
				uTime = ::atol(optarg);
				break;

			case 'u':
				url = ::strdup(optarg);
				break;

			case 'w':
				maxTime = atoi(optarg);
				break;

			default:
				printUsageAndExit(1);
				break;
			}
		}

		// First initialize the ORB, that will remove some arguments...
		SharedServiceMain::init( argc, argv, __FILE__, false );

		if (url == (const char *) 0)
		{
			if (appConfig->findString("GS_URL"));
			{
				url = appConfig->getString("GS_URL");
			}
		}

		if (url == (const char *) 0)
		{
			url = ::getenv("GS_URL");
		}

		if (url == (const char *) 0)
		{
			printUsageAndExit(1);
		}

		APPLOG_DBG("find GS_URL=%s", url);

		time_t startT = ::time((time_t *) 0);
		time_t nowT = startT;
		bool ok = false;

		while (!ok && ((nowT - startT) < maxTime))
		{
            try{
                CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve(url);
                if (obj == NULL)
                {
                    APPLOG_INFO("Couldn't resolve: %s", url);
                }
                else
                {
                    Service::GenericService_ptr gs = Service::GenericService::_narrow(obj);
                    ok = opPing(gs);
                }
            } catch (CORBA::SystemException &cse) {
                (void) ::fprintf(stderr, "caught CORBA::SystemException: %s\n", cse._info().rep());
                cse._tao_print_system_exception(stderr);
            }
			if (!ok)
			{
				(void) ::usleep(uTime);
				nowT = ::time((time_t *) 0);
			}
		}

		if (!ok)
		{
			retVal = 2;
		}
        retVal=0;
	} catch (Service::ServiceException &se) {
		(void) ::fprintf(stderr, "caught serviceException %s\n", se.what_string.in());
	} catch (CORBA::SystemException &se) {
		(void) ::fprintf(stderr, "caught CORBA::SystemException: %s\n", se._info().rep());
		se._tao_print_system_exception(stderr);
	} catch (exception &e) {
		(void) ::fprintf(stderr, "caught exception in main: %s\n", e.what());
	} catch (...) {
		(void) ::fprintf(stderr, "Caught something\n");
	}

	return retVal;
}
