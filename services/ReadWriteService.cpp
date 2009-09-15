
#include "ReadWriteServiceImpl.h"
#include "AppConfig.h"
#include "AppLog.h"
#include "AppExceptionHandler.h"
#include "SharedServiceMain.h"
#include "keys.h"


int main(int argc, char *argv[])
{
	static const char *name = RWS_NAME;

	try {
		//initialize singletons: AppConfig, AppLog, orb, root poa
		SharedServiceMain::init( argc, argv, name );

		//create CORBA servant for ReadWriteService
		clusterio_ReadWriteServiceImpl rw( * AppConfig::instance());
		APPLOG_DBG("repository id[%s]", rw._interface_repository_id());

		//activate CORBA servant, and register with name service
		SharedServiceMain::instance()->activate(&rw);
		SharedServiceMain::instance()->registerSelf();

		//main loop
		SharedServiceMain::instance()->run();
	}
	catch (CORBA::Exception &ce)
	{
		//AppExceptionHandler really needs to learn about CORBA...
		APPLOG_ABORT("ReadWriteService main() caught CORBA::Exception [%s][%s]",
			ce._rep_id(), ce._name() );
		exit(1);
	}
	catch (std::exception &e)
	{
		AppExceptionHandler::instance()->handleExit(e);
	}
	catch ( ... )
	{
		AppExceptionHandler::instance()->handleExit();
	}

	SharedServiceMain::shutdown();

	printf("Exiting from %s\n", name);

	return 0;
}
