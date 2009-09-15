
#ifndef SHUTDOWNHANDLER_H
#define SHUTDOWNHANDLER_H 1

#include "Exclusion.h"
#include "Runnable.h"

class ThreadRunnable;

class ShutdownHandler: public Runnable
{
public:
	enum ShutdownType { SOFT, HARD };

	ShutdownHandler();
	virtual ~ShutdownHandler();

	static ShutdownHandler *instance();
	static void startRunning();

	virtual void requestShutdown(ShutdownType t);
	virtual void run();

protected:
	static ShutdownHandler *instance_;
	static ThreadRunnable *thr_;

	typedef void (*SigHandler)(int);

	static void installHandler(int sig, SigHandler fn);
	static void sighandler_HUP(int sig);
	static void sighandler_TERM(int sig);

	Mutex lock_;
	Condition waitFor_;
	ShutdownType type_;

	virtual bool handleShutdown(ShutdownType t);
};

#endif
