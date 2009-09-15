//
// server.cpp,v 1.4 2001/03/29 19:42:06 coryan Exp
//

#include "JobQueueServiceImpl.h"
#include "SharedServiceMain.h"
#include "ShutdownHandler.h"
#include "keys.h"

class JQS_ShutdownHandler: public ShutdownHandler
{
public:
	JQS_ShutdownHandler(JobQueue_JobQueueServiceImpl &jqs) : jqs_(jqs) {};

	virtual bool handleShutdown(ShutdownHandler::ShutdownType t);

private:
	JobQueue_JobQueueServiceImpl &jqs_;
};


bool
JQS_ShutdownHandler::handleShutdown(ShutdownHandler::ShutdownType t)
{
	bool retVal = false;

	try {
		switch (t) {
		case ShutdownHandler::SOFT:
			jqs_.gracefulShutdown();
			break;

		case ShutdownHandler::HARD:
			retVal = true;
			jqs_.shutdownNow();
			break;
		}
		retVal = true;
	} catch (...) {
		try {
			APPLOG_WARN("Caught something when shutting down via %d", t);
		} catch (...) {
			//@@TODO: What should we do now?
		}
	}

	if (retVal)
	{
		retVal = ShutdownHandler::handleShutdown(t);
	}

	return retVal;
};


static PortableServer::Servant 
jqsFactory()
{
    static JobQueue_JobQueueServiceImpl jqs(*AppConfig::instance());
	static JQS_ShutdownHandler *handler = new JQS_ShutdownHandler(jqs);

	handler->startRunning();

    return &jqs;
};

int main (int argc, char* argv[])
{
    //name will serve as the default GS_SERVICENAME
    return SharedServiceMain::serviceMain(
        argc, argv, JQS_NAME, jqsFactory, 
        true, true, JQS_IOR_FILE, JE_NAME );
};

