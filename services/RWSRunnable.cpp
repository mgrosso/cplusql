
#include "DateUtils.h"
#include "Exclusion.h"
#include "fwrap.h"
#include "netwrap.h"
#include "ReadWriteController.h"
#include "RWSExceptions.h"
#include "RWSRunnable.h"
#include "SharedServiceMain.h"


using namespace rws;

void
RWSRunnable::handleException(clusterio::ClusterIOException &cie)
{
	MutexGuard guard(mtx_);
	int newSz = cie.exceptions.length();
	int eSz = cie_.exceptions.length();

	cie_.exceptions.length(eSz + newSz);
	for (int i = 0; i < newSz; i++)
	{
		cie_.exceptions[i + eSz] = cie.exceptions[i];
	}
}


void
RWSRunnable::handleException(Service::ServiceException &se)
{
	MutexGuard guard(mtx_);
	int sz = cie_.exceptions.length();

	cie_.exceptions.length(sz + 1);
	cie_.exceptions[sz].serviceRef = ref_;
	cie_.exceptions[sz].type = clusterio::serviceNotFound;
	cie_.exceptions[sz].errno_short = se.errno_short;
	cie_.exceptions[sz].what_string = se.what_string;
}


void
RWSRunnable::handleException(CORBA::SystemException &se)
{
	MutexGuard guard(mtx_);
	int sz = cie_.exceptions.length();

	cie_.exceptions.length(sz + 1);
	cie_.exceptions[sz].serviceRef = ref_;
	cie_.exceptions[sz].type = clusterio::unknown;
	cie_.exceptions[sz].errno_short = 0;

	//@@TODO: there is also a _info().
	cie_.exceptions[sz].what_string = se._name();
}


void
RWSRunnable::handleException(std::exception &e)
{
	MutexGuard guard(mtx_);
	int sz = cie_.exceptions.length();

	cie_.exceptions.length(sz + 1);
	cie_.exceptions[sz].serviceRef = ref_;
	cie_.exceptions[sz].type = clusterio::unknown;
	cie_.exceptions[sz].errno_short = 0;
	cie_.exceptions[sz].what_string = e.what();
}


void
RWSRunnable::handleException()
{
	MutexGuard guard(mtx_);
	char_ptr s = rwc_.dupSequence(wid_.get_buffer(), wid_.length());
	int sz = cie_.exceptions.length();

	cie_.exceptions.length(sz + 1);
	cie_.exceptions[sz].serviceRef = ref_;
	cie_.exceptions[sz].type = clusterio::unknown;
	cie_.exceptions[sz].errno_short = 0;
	cie_.exceptions[sz].what_string = s.get();
}



AppendRunnable::AppendRunnable(ReadWriteController &rwc, clusterio::ReadWriteService_ptr rws, Mutex &mtx, clusterio::ClusterIOException &cie, const Service::ServiceRef &ref, const clusterio::file_handle &handle, const clusterio::blockseq &blocks, const clusterio::writer_id &wid, CORBA::ULong seqNum)
	: RWSRunnable(rwc, mtx, cie, ref, wid), rws_(rws), handle_(handle), blocks_(blocks), seqNum_(seqNum)
{
}


void
AppendRunnable::run()
{
	APPLOG_INFO("appendRunnable::run for %s", wid_.get_buffer());

	(void) ::gettimeofday(&startT_, 0);

	try {
		clusterio::open_info oi;

		oi.handle = handle_;
		oi.servants.length(0);
		rws_->append(oi, blocks_, wid_, seqNum_);
	} catch (clusterio::ClusterIOException &cie) {
		APPLOG_ALARM("%ld caught cie for %s", __LINE__, wid_.get_buffer());
		handleException(cie);
	} catch (Service::ServiceException &se) {
		APPLOG_ALARM("%ld caught s:se for %s", __LINE__, wid_.get_buffer());
		handleException(se);
	} catch (CORBA::SystemException &se) {
		APPLOG_ALARM("%ld caught c:se for %s %s", __LINE__, wid_.get_buffer(), se._info().rep());
		handleException(se);
	} catch (std::exception &e) {
		APPLOG_ALARM("%ld caught c:e for %s", __LINE__, wid_.get_buffer());
		handleException(e);
	} catch (...) {
		APPLOG_ALARM("%ld caught something for %s", __LINE__, wid_.get_buffer());
		handleException();
	}

	DateUtils::getTime(endT_);
	double totalT = DateUtils::diffTime(startT_, &endT_);
	APPLOG_INFO("append took %8.4fs", totalT);
}



UndoRunnable::UndoRunnable(ReadWriteController &rwc, clusterio::ReadWriteService_ptr rws, Mutex &mtx, clusterio::ClusterIOException &cie, const Service::ServiceRef &ref, const clusterio::file_handle &fh, const clusterio::writer_id &wid)
	: RWSRunnable(rwc, mtx, cie, ref, wid), rws_(rws), handle_(fh), seqNum_(0), bytesUndone_(0)
{
}


void
UndoRunnable::run()
{
	APPLOG_INFO("appendRunnable::run for %s", wid_.get_buffer());

	(void) ::gettimeofday(&startT_, 0);

	try {
		clusterio::open_info oi;

		oi.handle = handle_;
		oi.servants.length(0);

		bytesUndone_ = rws_->undo_appends(oi, wid_, seqNum_);
	} catch (clusterio::ClusterIOException &cie) {
		APPLOG_ALARM("%ld caught cie for %s", __LINE__, wid_.get_buffer());
		handleException(cie);
	} catch (Service::ServiceException &se) {
		APPLOG_ALARM("%ld caught s:se for %s", __LINE__, wid_.get_buffer());
		handleException(se);
	} catch (CORBA::SystemException &se) {
		APPLOG_ALARM("%ld caught c:se for %s", __LINE__, wid_.get_buffer());
		handleException(se);
	} catch (std::exception &e) {
		APPLOG_ALARM("%ld caught std::e for %s", __LINE__, wid_.get_buffer());
		handleException(e);
	} catch (...) {
		APPLOG_ALARM("%ld caught something for %s", __LINE__, wid_.get_buffer());
		handleException();
	}
	DateUtils::getTime(endT_);
	double totalT = DateUtils::diffTime(startT_, &endT_);
	APPLOG_INFO("undo took %8.4fs", totalT);
}


ServerSocketReader::~ServerSocketReader()
{
	if (inFd_ != -1)
	{
		netwrap::closeClient(inFd_);
		inFd_ = -1;
	}
	if (serverSock_ != -1) {
		netwrap::closeServer(serverSock_);
		serverSock_ = -1;
	}
}


void
ServerSocketReader::run()
{
	MutexGuard guard(mtx_);

	(void) ::gettimeofday(&startT_, 0);

	try {
		port_ = 0;
		serverSock_ = netwrap::createServer(port_, host_);

		APPLOG_INFO("running port=%d, socket=%d", port_, serverSock_);
		if (serverSock_ >= 0)
		{
			struct sockaddr_in clientEnd;

			APPLOG_INFO("going to wait for client on socket fd %d", serverSock_);
			inFd_ = netwrap::waitForClient(serverSock_, (struct sockaddr *) &clientEnd);

			APPLOG_DBG("waitForClient returned %d", inFd_);

			if (inFd_ >= 0)
			{
				int numRd;
				char buf[blkSz_];

				while ((numRd = fwrap::read(inFd_, buf, blkSz_)) > 0)
				{
					int numWr = fwrap::write(toFd_, buf, numRd);

					APPLOG_DBG("port %d: read %ld, fd %d: wrote %ld", port_,
						(long) numRd, toFd_, (long) numWr);

					numRd_ += numRd;
					if (numWr > 0)
					{
						numWr_ += numWr;
					}
					if (numWr != numRd)
					{
						throw WriteException(NICE_LOC, Resource(host_, port_), Args(numRd_, numRd, numWr), Status(errno, "Failed to write"));
					}
				}
				if (numRd == -1)
				{
					throw ReadException(NICE_LOC, Resource(host_, port_), Args(numRd_, blkSz_, numRd), Status(errno, "Failed to read from socket"));
				}
				netwrap::closeClient(inFd_);
				inFd_ = -1;
			}
			else
			{
				APPLOG_ALARM("Failed to accept client %d", serverSock_);
			}
			netwrap::closeServer(serverSock_);
			serverSock_ = -1;
		}
	} catch (rws::Exception &e) {
		const char *msg = e.what();

		APPLOG_ALARM("%ld caught rws:e for", __LINE__, msg);
		se_ = Service::ServiceException(__LINE__, msg);
	} catch (std::exception &e) {
		const char *msg = e.what();

		APPLOG_ALARM("%ld caught c:e", __LINE__, msg);
		se_ = Service::ServiceException(__LINE__, msg);
	} catch (...) {
		APPLOG_ALARM("%ld caught something", __LINE__);
		se_ = Service::ServiceException(__LINE__, __PRETTY_FUNCTION__);
	}

	DateUtils::getTime(endT_);
	double totalT = DateUtils::diffTime(startT_, &endT_);
	APPLOG_INFO("copy took %8.4fs", totalT);
}
