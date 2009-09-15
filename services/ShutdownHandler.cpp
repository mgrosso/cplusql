
#include <signal.h>

#include "AppLog.h"
#include "SharedServiceMain.h"
#include "ShutdownHandler.h"
#include "ThreadRunnable.h"


ShutdownHandler *ShutdownHandler::instance_ = (ShutdownHandler *) 0;
ThreadRunnable *ShutdownHandler::thr_ = (ThreadRunnable *) 0;


ShutdownHandler::ShutdownHandler()
{
	if (instance_ == (ShutdownHandler *) 0)
	{
		installHandler(SIGTERM, sighandler_TERM);
		installHandler(SIGHUP, sighandler_HUP);

		instance_ = this;
	}
}


ShutdownHandler::~ShutdownHandler()
{
	if (this == instance_)
	{
		installHandler(SIGTERM, SIG_DFL);
		installHandler(SIGHUP, SIG_DFL);
		instance_ = (ShutdownHandler *) 0;
	}
}


void
ShutdownHandler::installHandler(int sig, ShutdownHandler::SigHandler fn)
{
	struct sigaction act;

	act.sa_handler = fn;
	act.sa_flags = SA_ONSTACK | SA_RESTART;

	int st = sigaction(sig, &act, (struct sigaction *) 0);

	if (st != 0)
	{
		APPLOG_WARN("Couldn't install sig handler for %d", sig);
	}
}


ShutdownHandler *
ShutdownHandler::instance()
{
	//@@TODO Check
	//
	return ShutdownHandler::instance_;
}


void
ShutdownHandler::startRunning()
{
	if (thr_ == (ThreadRunnable *) 0)
	{
		RunnablePtr r = instance_;
		thr_ = new ThreadRunnable(r);
		thr_->start();
	}
}


void
ShutdownHandler::sighandler_HUP(int)
{
	ShutdownHandler::instance()->requestShutdown(SOFT);
}


void
ShutdownHandler::sighandler_TERM(int)
{
	ShutdownHandler::instance()->requestShutdown(HARD);
}


void
ShutdownHandler::requestShutdown(ShutdownType t)
{
	type_ = t;
	waitFor_.signal();
}


void
ShutdownHandler::run()
{
	APPLOG_INFO("ShutdownHandler running");
	MutexGuard guard(lock_);

	APPLOG_INFO("ShutdownHandler waiting");
	waitFor_.wait(guard);
	APPLOG_INFO("ShutdownHandler after wait; type=%d", type_);

	bool handled = handleShutdown(type_);

	if (!handled)
	{
		APPLOG_WARN("shutdown(%d) now handled propertly", type_);
	}
}


bool
ShutdownHandler::handleShutdown(ShutdownType t)
{
	bool retVal = true;

	APPLOG_INFO("ShutdownHandler type=%d", type_);

	try {
		switch (t) {
		case SOFT:
			SharedServiceMain::instance()->shutdown(false);
			break;

		case HARD:
			SharedServiceMain::instance()->shutdown(true);
			break;
		}
	} catch (...) {
		retVal = false;
	}

	return retVal;
}
