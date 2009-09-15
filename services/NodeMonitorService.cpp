//
// server.cpp,v 1.4 2001/03/29 19:42:06 coryan Exp
//

#include "NodeMonitorServiceImpl.h"
#include "SharedServiceMain.h"
#include "ShutdownHandler.h"
#include "keys.h"


class NMS_ShutdownHandler: public ShutdownHandler
{
public:
    NMS_ShutdownHandler(Service_NodeMonitorServiceImpl &nms) : nms_(nms) {};

    virtual bool handleShutdown(ShutdownHandler::ShutdownType t);

private:
    Service_NodeMonitorServiceImpl &nms_; 
};


bool
NMS_ShutdownHandler::handleShutdown(ShutdownHandler::ShutdownType t)
{
	bool retVal = false;

	try {
		switch (t) {
		case ShutdownHandler::SOFT:
			nms_.gracefulShutdown();
			break;

		case ShutdownHandler::HARD:
			retVal = true;
			// We don't do a gracefulShutdown(), since we're supposed to
			// slam the server (ourself), not be nice.
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
nmsFactory()
{
    static Service_NodeMonitorServiceImpl nms(*AppConfig::instance());
	static NMS_ShutdownHandler *handler = new NMS_ShutdownHandler(nms);

	handler->startRunning();

    return &nms;
};

int main (int argc, char* argv[])
{
    return SharedServiceMain::serviceMain(
        argc, argv, NMS_NAME, nmsFactory );
};
