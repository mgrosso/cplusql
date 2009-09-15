
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "clusterioC.h"
#include "DestFile.h"
#include "fwrap.h"
#include "keys.h"
#include "netwrap.h"
#include "NVPairUtil.h"
#include "RWSRunnable.h"
#include "ReadWriteController.h"
#include "ServiceInfo.h"
#include "SharedServiceMain.h"
#include "ThreadRunnable.h"


bool
ReadWriteController::HandlerCache::isSafeToRemove(const FileHandler *fh) const
{
	bool retVal = false;

	if (fh != (const FileHandler *) 0)
	{
		int cnt = fh->getCurrentUserCount();

		retVal = (cnt == 0);
	}

	return retVal;
}


ReadWriteController::ReadWriteController(const Config &cfg)
	: cfg_(cfg), logger_((DestFile *) 0), diskInfo_((DiskInfo *) 0),
	netInfo_((NetInfo *) 0), procInfo_((ProcessInfo *) 0),
	sysInfo_((SystemInfo *) 0), fhCache_((HandlerCache *) 0)
{
	(void) ::gettimeofday(&startT_, 0);
	init();
}


void
ReadWriteController::init()
{
	baseDir_ = ptr_strdup(cfg_.getString("BASE_DIR"));
	if (procInfo_ == (ProcessInfo *) 0)
	{
		procInfo_ = new ProcessInfo();
	}
	if (sysInfo_ == (SystemInfo *) 0)
	{
		sysInfo_ = new SystemInfo();
	}
	if (diskInfo_ != (DiskInfo *) 0)
	{
		delete diskInfo_;
	}
	diskInfo_ = new DiskInfo(baseDir_.get());

	const char *nic = (const char *) 0;

	if (cfg_.findString("RWS_CP_NIC"))
	{
 		nic = cfg_.getString("RWS_CP_NIC");
	}

	if (nic == (const char *) 0)
	{
		nic = sysInfo_->getHostname();
	}
	nic_ = ptr_strdup(nic);

	if (netInfo_ != (NetInfo *) 0)
	{
		delete netInfo_;
	}
	netInfo_ = new NetInfo(nic_.get());

	APPLOG_INFO("starting pid=%ld", procInfo_->getPid());

	if (cfg_.findString("MASTER_LOG_FILE"))
	{
		logName_ = ptr_strdup(cfg_.getString("MASTER_LOG_FILE"));
	}
	else
	{
		logName_ = fwrap::mkFilePath(baseDir_.get(), "rws", ".log");
	}

	int ok = !(fwrap::mkdirp(logName_.get(), 0755));

	if (!ok)
	{
		APPLOG_ALARM("Couldn't mkdirp for %s", logName_.get());
	}

	if (logger_ != (DestFile *) 0)
	{
		delete logger_;
	}
	logger_ = new DestFile(logName_.get());

	int maxCacheSize = cfg_.getInt(RWS_MAX_CACHE_SIZE, "256");
	int maxFd = cfg_.getInt(RWS_MAX_NOFILES, "1024");

	if (fhCache_ == (HandlerCache *) 0)
	{
		fhCache_ = new HandlerCache(maxCacheSize);
	}
	procInfo_->setMaxFD(maxFd);
}


ReadWriteController::~ReadWriteController()
{
	if (logger_ != (DestFile *) 0)
	{
		struct timeval endT;

		::gettimeofday(&endT, 0);
		APPLOG_INFO("%ld: %d %s stopping\n", endT.tv_sec, procInfo_->getPid(), sysInfo_->getHostname());

		delete logger_;
		logger_ = (DestFile *) 0;
	}

	if (fhCache_ != (HandlerCache *) 0)
	{
		delete fhCache_;
		fhCache_ = (HandlerCache *) 0;
	}

	if (diskInfo_ != (DiskInfo *) 0)
	{
		delete diskInfo_;
		diskInfo_ = (DiskInfo *) 0;
	}

	if (netInfo_ != (NetInfo *) 0)
	{
		delete netInfo_;
		netInfo_ = (NetInfo *) 0;
	}

	if (sysInfo_ != (SystemInfo *) 0)
	{
		delete sysInfo_;
		sysInfo_ = (SystemInfo *) 0;
	}

	if (procInfo_ != (ProcessInfo *) 0)
	{
		delete procInfo_;
		procInfo_ = (ProcessInfo *) 0;
	}
}


int
ReadWriteController::getCacheMaxSize() const throw ()
{
	int retVal = -1;

	if (fhCache_ != (HandlerCache *) 0)
	{
		retVal = fhCache_->getMaxSize();
	}

	return retVal;
}


int
ReadWriteController::getCacheCurrentSize() const throw ()
{
	int retVal = -1;

	if (fhCache_ != (HandlerCache *) 0)
	{
		retVal = fhCache_->getCurrentSize();
	}

	return retVal;
}


void
ReadWriteController::setCacheMaxSize(int sz)
{
	if (fhCache_ != (HandlerCache *) 0)
	{
		fhCache_->setMaxSize(sz);
	}
}


void
ReadWriteController::mapException(const rws::Exception &e, clusterio::ClusterIOException &ce) throw ()
{
	int sz = ce.exceptions.length();

	ce.exceptions.length(sz + 1);

	mapException(e, ce.exceptions[sz]);
}

void
ReadWriteController::mapException(const rws::Exception &e, clusterio::io_exception_struct &ies) throw ()
{
	SharedServiceMain::instance()->initServiceRef(ies.serviceRef);
	ies.errno_short = e.getStatus().getErrno();
	ies.what_string = e.what();

	const char *cls = e.getClass();
	if (::strcmp(cls, rws::SpaceException::CLASS) == 0)
	{
		ies.type = clusterio::diskFull;
	}
	else if (::strcmp(cls, rws::OpenException::CLASS) == 0)
	{
		ies.type = clusterio::fileNotFound;
	}
	else if (::strcmp(cls, rws::ReadException::CLASS) == 0)
	{
		ies.type = clusterio::readError;
	}
	else if (::strcmp(cls, rws::WriteException::CLASS) == 0)
	{
		ies.type = clusterio::writeError;
	}
	else if (::strcmp(cls, rws::FileModeException::CLASS) == 0)
	{
		ies.type = clusterio::invalidParameter;
	}
	else if (::strcmp(cls, rws::StaleFileException::CLASS) == 0)
	{
		ies.type = clusterio::staleFile;
	}
	else if (::strcmp(cls, rws::BadDataException::CLASS) == 0)
	{
		ies.type = clusterio::invalidParameter;
	}
	else if (::strcmp(cls, rws::CopyException::CLASS) == 0)
    {
		ies.type = clusterio::writeError;
	}
	else if (::strcmp(cls, rws::ThreadException::CLASS) == 0)
    {
		ies.type = clusterio::invalidParameter;
	}
	else
	{
		//@@TODO: I don't think this is right.
		ies.type = clusterio::unknown;
	}

	APPLOG_INFO("mapException type=%d str=%s", ies.type, e.what());
}



void
ReadWriteController::mapException(const std::exception *e, clusterio::ClusterIOException &ce, const char *where) throw ()
{
	int sz = ce.exceptions.length();

	ce.exceptions.length(sz + 1);
	mapException(e, ce.exceptions[sz], where);
}


void
ReadWriteController::mapException(const std::exception *e, clusterio::io_exception_struct &ies, const char *where) throw ()
{
	SharedServiceMain::instance()->initServiceRef(ies.serviceRef);
	ies.errno_short = 0;

	if (e == (const std::exception *) 0)
	{
		ies.what_string = where;
		ies.type = clusterio::unknown;
	}
	else
	{
		ies.what_string = e->what();
		ies.type = clusterio::unknown;
	}

	APPLOG_INFO("mapException(std:e=%d str=%s", ies.type, ies.what_string.in());
}


char_ptr
ReadWriteController::convertPrivateNameToPath(clusterio::length_t id)
{
	char buf[128];
	char *bufP = buf;
	union {
		clusterio::length_t l;
		unsigned char b[sizeof(clusterio::length_t)];
	} munge;

	buf[0] = '\0';
	munge.l = netwrap::htonll(id);

	// Now map the 8 bytes of each long long into a dir structure like bb/bb/bb/bb
	//
	for (size_t i = 0; i < sizeof(munge.b); i += 2)
	{
		if (i > 0)
		{
			*bufP++ = '/';
		}

		int numWr = ::sprintf(bufP, "%02x%02x", (int) munge.b[i], (int) munge.b[i + 1]);
		bufP += numWr;
	}

	*bufP++ = '\0';

	char_ptr retVal = ptr_strdup(buf);

	return retVal;
}



int
ReadWriteController::getCurrentUserCount() const
{
	int retVal = 0;

	semCtr_.getvalue(&retVal);
	retVal -= 1;

	return retVal;
}


FileHandler *
ReadWriteController::getFileHandler(long id, const char *path)
{
	MutexGuard guard(mtx_);
	FileHandler *retVal = fhCache_->get(path);

	if (retVal == (FileHandler *) 0)
	{
		retVal = new FileHandler(*this, id, path);

		if (retVal != (FileHandler *) 0)
		{
			retVal->init();
			fhCache_->add(path, retVal, 1);
			logVar("%lld: %s %s", (long long) id, path);
		}
	}

	return retVal;
}


FileHandler *
ReadWriteController::getFileHandler(const clusterio::file_handle &fd, const char *wid)
{
	char_ptr path = convertPrivateNameToPath(fd.id);
	MutexGuard guard(mtx_);
	FileHandler *retVal = fhCache_->get(path.get());

	if (retVal == (FileHandler *) 0)
	{
		const char *p = strdup_wnew(path.get());
		const char *d = fd.debug_info.in();

		if (wid == (const char *) 0)
		{
			wid = "";
		}

		APPLOG_DBG("%s: creating FH(%s) %s", wid, p, d);
		retVal = new FileHandler(*this, fd.id, p);

		if (retVal != (FileHandler *) 0)
		{
			retVal->init();
			fhCache_->add(p, retVal, 1);
			logVar("%lld: %s %s", (long long) fd.id, path.get(), fd.debug_info.in());
		}
	}

	return retVal;
}


char_ptr
ReadWriteController::dupSequence(const TAO_Unbounded_Sequence<CORBA::Octet> &seq, size_t &len)
{
	len = seq.length();

	char_ptr retVal = dupSequence(seq.get_buffer(), len);

	return retVal;
}


char_ptr
ReadWriteController::dupSequence(const CORBA::Octet *p, size_t len)
{
	const char *s = reinterpret_cast<const char *>(p);
	char *nS = new char[len + 1];

	::memcpy(nS, s, len);
	nS[len] = '\0';

	char_ptr retVal = ptr_strdup_take(nS);

	return retVal;
}


void
ReadWriteController::verifyOpenFlags(const clusterio::open_info &fd,
	unsigned long req) throw (rws::FileModeException)
{
	unsigned long flag = fd.handle.flags;

	if ((flag & req) != req)
	{
		using namespace rws;

		throw FileModeException(NICE_LOC, Resource(fd.handle.debug_info.in(), 0), Args(0, flag, req), Status("Wrong open flags"));
	}
}


void
ReadWriteController::append(const clusterio::open_info &fd,
	const clusterio::blockseq &blocks, const clusterio::writer_id &writer,
	CORBA::ULong seqNum)
	throw (clusterio::ClusterIOException, Service::ServiceException)
{
	Counter ctr(semCtr_);
	clusterio::ClusterIOException cie;

	APPLOG_DBG("cache size=%d", fhCache_->getCurrentSize());

	FAIL_RWC_IF("rwcAppendSleep", ::sleep(FAIL_RWC_INT("rwcAppendSleep")));

	int sz = fd.servants.length();
	int cnt = 0;
	Mutex mtx;
	RunnablePtr ar[sz];
	ptr<ThreadRunnable> tr[sz];

	// First, start the send to any secondary servers, as that involves
	// networking I/O + disk I/O
	//
	if (sz > 0)
	{
		Service::ServiceRef myRef;

		SharedServiceMain::instance()->initServiceRef(myRef);
		APPLOG_DBG("Secondary servers=%d", sz);

		for (int i= 0; i < sz; i++)
		{
			const Service::ServiceRef &ref = fd.servants[i];

			if (::strcmp(myRef.url.in(), ref.url.in()) != 0)
			{
				CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve(ref.url);
	
				if (obj == (CORBA::Object *) 0)
				{
					throw Service::ServiceException(0, ref.url);
				}
				clusterio::ReadWriteService_ptr rws = clusterio::ReadWriteService::_narrow(obj);

				ar[cnt] = new AppendRunnable(*this, rws, mtx, cie, ref, fd.handle, blocks, writer, seqNum);
				tr[cnt] = new ThreadRunnable(ar[cnt]);

				tr[cnt].get()->start();
				cnt++;
			}
		}
	}

	// Now write to our own disk.
	//
	try {
		verifyOpenFlags(fd, clusterio::o_wronly);

		char_ptr writerId = dupSequence(writer.get_buffer(), writer.length());

		FileHandler *handler = getFileHandler(fd.handle, writerId.get());

		if (handler != (FileHandler *) 0)
		{
			//@@TODO: if we want to support aio_, we should split this up into
			// an append_start/append_end.
			handler->append(blocks, writerId.get(), seqNum);
		}
	} catch (rws::Exception &e) {
		MutexGuard guard(mtx);

		APPLOG_ALARM("rws::e: %s\n", e.what());
		mapException(e, cie);
	} catch (exception &e) {
		MutexGuard guard(mtx);

		APPLOG_ALARM("Caught an exception %s", e.what());
		mapException(&e, cie, __PRETTY_FUNCTION__);
	} catch ( ... ) {
		MutexGuard guard(mtx);

		APPLOG_ALARM("Caught something");
		mapException((const std::exception *) 0, cie, __PRETTY_FUNCTION__);
	}

	// Now wait for the secondary servers to finish their write.
	//
	for (int i = 0; i < cnt; i++)
	{
		AsyncRunnable::erunstate thrState = tr[i]->wait();

		APPLOG_DBG("waited on secondary append[%d] state==%d", i, thrState);
	}

	if (cie.exceptions.length() > 0)
	{
		throw cie;
	}
}


::clusterio::file_handle_status_seq *
ReadWriteController::stat(const clusterio::file_handle_seq &fdset, CORBA::Boolean checksums)
	throw (CORBA::SystemException, Service::ServiceException)
{
	Counter ctr(semCtr_);

	FAIL_RWC_IF("rwcStatSleep", ::sleep(FAIL_RWC_INT("rwcStatSleep")));

	clusterio::file_handle_status_seq *retVal = new clusterio::file_handle_status_seq();

	int sz = fdset.length();
	retVal->length(sz);

	APPLOG_DBG("rwc.stat: want info on %d files", sz);

	// Go through all the requested files.
	//
	for (int i = 0; i < sz; i++)
	{
		const clusterio::file_handle &cfh = fdset[i];
		clusterio::file_handle_status fhStat;
		FileHandler *fh = getFileHandler(cfh);

		fhStat.id = cfh.id;
		if ((fh == (FileHandler *) 0) || !fh->isPresent())
		{
			APPLOG_DBG("%d: no file", i);
			fhStat.status = clusterio::not_found;
		}
		else if (!checksums)
		{
			APPLOG_DBG("%d: data not requested, so not returning blocks");
			fhStat.status = clusterio::normal;
		}
		else
		{
			APPLOG_DBG("stat(%s)", fh->getFilename());

			int cnt = 0;
			list<AuditLog::Entry> *l = fh->getValidEntries();

			if (l == (list<AuditLog::Entry> *) 0)
			{
				fhStat.status = clusterio::not_found;
			}
			else
			{
				list<AuditLog::Entry>::iterator itor = l->begin();

				APPLOG_DBG("size=%ld", (long) l->size());

				fhStat.data_ranges.length(l->size());
				fhStat.id = fh->getPrivateId();
				fhStat.status = clusterio::normal;

				while (itor != l->end())
				{
					AuditLog::Entry &e = *itor;
					clusterio::data_range dr;

					dr.offset = e.offset;
					dr.length = e.len;
					dr.checksum.length(clusterio::clusterio_checksum_size);
					for (int i = 0; i < clusterio::clusterio_checksum_size; i++)
						dr.checksum[i] = e.checksum[i];
					dr.seconds = e.startT.tv_sec;
					dr.useconds = e.startT.tv_usec;
					dr.writers_sequence = e.seqNo;
					dr.writer.length(clusterio::writer_id_len);
	
					APPLOG_DBG("stat block %d: offset=%lld len=%ld", cnt, (long long) e.offset, (long) e.len);
					e.copyTo(dr.writer);

					fhStat.check_type= e.checksumType;
					fhStat.data_ranges[cnt++] = dr;

					itor++;
				}
			}
		}

		(*retVal)[i] = fhStat;
	}

	return retVal;
}


clusterio::length_t
ReadWriteController::undo_appends(const clusterio::open_info &fd,
	const clusterio::writer_id &writer, CORBA::ULong_out seqNum)
throw (clusterio::ClusterIOException, Service::ServiceException)
{
	clusterio::length_t retVal = 0;
	Counter ctr(semCtr_);
	clusterio::ClusterIOException cie;

	FAIL_RWC_IF("rwcUndoAppendsSleep", ::sleep(FAIL_RWC_INT("rwcUndoAppendsSleep")));

	int sz = fd.servants.length();
	int cnt = 0;
	Mutex mtx;
	RunnablePtr ar[sz];
	ptr<ThreadRunnable> tr[sz];

	// First, start the send to any secondary servers, as that involves
	// networking I/O + disk I/O
	//
	if (sz > 0)
	{
		Service::ServiceRef myRef;

		SharedServiceMain::instance()->initServiceRef(myRef);
		APPLOG_DBG("Secondary servers=%d", sz);

		for (int i= 0; i < sz; i++)
		{
			const Service::ServiceRef &ref = fd.servants[i];

			if (::strcmp(myRef.url.in(), ref.url.in()) != 0)
			{
				CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve(ref.url);
	
				if (obj == (CORBA::Object *) 0)
				{
					throw Service::ServiceException(0, ref.url);
				}
				clusterio::ReadWriteService_ptr rws = clusterio::ReadWriteService::_narrow(obj);

				ar[cnt] = new UndoRunnable(*this, rws, mtx, cie, ref, fd.handle, writer);
				tr[cnt] = new ThreadRunnable(ar[cnt]);

				APPLOG_DBG("starting secondary undo[%d] to %s", cnt, ref.url.in());
				tr[cnt].get()->start();
				cnt++;
			}
		}
	}

	// Now write to our own disk.
	//
	try {
		char_ptr writerId = dupSequence(writer.get_buffer(), writer.length());
		FileHandler *handler = getFileHandler(fd.handle, writerId.get());

		if (handler != (FileHandler *) 0)
		{
			retVal = handler->undo_appends(writerId.get(), seqNum);
		}
	} catch (rws::Exception &e) {
		MutexGuard guard(mtx);

		APPLOG_ALARM("rws::e: %s\n", e.what());
		mapException(e, cie);
	} catch (exception &e) {
		MutexGuard guard(mtx);

		APPLOG_ALARM("Caught an exception %s", e.what());
		mapException(&e, cie, __PRETTY_FUNCTION__);
	} catch ( ... ) {
		MutexGuard guard(mtx);

		APPLOG_ALARM("Caught something");
		mapException((const std::exception *) 0, cie, __PRETTY_FUNCTION__);
	}

	// Now wait for the secondary servers to finish their write.
	//
	for (int i = 0; i < cnt; i++)
	{
		AsyncRunnable::erunstate thrState = tr[i]->wait();
		Runnable *r = ar[i].get();
		UndoRunnable *ur = reinterpret_cast<UndoRunnable *>(r);

		APPLOG_DBG("waited on secondary undo[%d] state==%d", i, thrState);

		if (ur->getBytesUndone() != retVal)
		{
			APPLOG_ALARM("undid %lld bytes vs %lld for local", ur->getBytesUndone(), retVal);
			retVal = max(retVal, ur->getBytesUndone());
		}
		if (ur->getNextSeqNum() != seqNum)
		{
			APPLOG_ALARM("undo[i] nextSeq=%ld vs %ld for local", ur->getNextSeqNum(), seqNum);
			seqNum = max(seqNum, ur->getNextSeqNum());
		}
	}

	if (cie.exceptions.length() > 0)
	{
		throw cie;
	}

	return retVal;
}


void
ReadWriteController::cp(const clusterio::file_handle &fd,
	const Service::ServiceRef &source, const clusterio::file_handle &sourcefd)
    throw (clusterio::ClusterIOException, Service::ServiceException)
{
	Counter ctr(semCtr_);

	FAIL_RWC_IF("rwcCpSleep", ::sleep(FAIL_RWC_INT("rwcCpSleep")));

	try {
		FileHandler *handler = getFileHandler(fd, (const char *) 0);

		if (handler != (FileHandler *) 0)
		{
			APPLOG_DBG("starting a copyTo");

			CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve(source.url);
    
			if (obj == (CORBA::Object *) 0)
			{
				throw Service::ServiceException(0, source.url);
			}
			clusterio::ReadWriteService_ptr rws = clusterio::ReadWriteService::_narrow(obj);

			APPLOG_DBG("host is %s", nic_.get());
			handler->setAppendToken(fd.append_token);
			handler->copyFrom(rws, sourcefd, nic_.get());
		}
	} catch (rws::Exception &e) {
		APPLOG_ALARM("rws::e: %s\n", e.what());
		throw Service::ServiceException(e.getStatus().getErrno(), e.what());
	} catch (std::exception &e) {
		APPLOG_ALARM("std::e: %s\n", e.what());
		throw Service::ServiceException(__LINE__, e.what());
	} catch ( ... ) {
		APPLOG_ALARM("caught something");
		throw Service::ServiceException(0, __PRETTY_FUNCTION__);
	}
}


void
ReadWriteController::cp(const clusterio::file_handle &fd,
	const char *destHost, CORBA::Long destFilePort, CORBA::Long destAuditPort)
	throw (Service::ServiceException)
{
	Counter ctr(semCtr_);

	FAIL_RWC_IF("rwcCpSleep", ::sleep(FAIL_RWC_INT("rwcCpSleep")));

	APPLOG_DBG("copyTo(host=%s, fileport=%d, auditport=%d", destHost, destFilePort, destAuditPort);

	try {
		FileHandler *handler = getFileHandler(fd, (const char *) 0);

		if (handler != (FileHandler *) 0)
		{
			handler->setAppendToken(fd.append_token);
			handler->copyTo(destHost, (int) destFilePort, (int) destAuditPort);
		}
	} catch (rws::Exception &e) {
		APPLOG_ALARM("rws::e: %s\n", e.what());
		throw Service::ServiceException(e.getStatus().getErrno(), e.what());
	} catch (std::exception &e) {
		APPLOG_ALARM("std::e: %s\n", e.what());
		throw Service::ServiceException(0, e.what());
	} catch ( ... ) {
		APPLOG_ALARM("caught something");
		throw Service::ServiceException(0, __PRETTY_FUNCTION__);
	}
}


void
ReadWriteController::setAppendToken(const clusterio::file_handle &fd,
	CORBA::Long token)
	throw (Service::ServiceException)
{
	Counter ctr(semCtr_);

	FAIL_RWC_IF("rwcTokenSleep", ::sleep(FAIL_RWC_INT("rwcTokenSleep")));

	try {
		FileHandler *handler = getFileHandler(fd, (const char *) 0);

		if (handler != (FileHandler *) 0)
		{
			handler->setAppendToken(token);
		}
	} catch (rws::Exception &e) {
		throw Service::ServiceException(e.getStatus().getErrno(), e.what());
	} catch (exception &e) {
		throw Service::ServiceException(0, e.what());
	} catch ( ... ) {
		throw Service::ServiceException(0, __PRETTY_FUNCTION__);
	}
}


void
ReadWriteController::rm(const clusterio::file_handle &fd)
	throw (Service::ServiceException)
{
	Counter ctr(semCtr_);

	FAIL_RWC_IF("rwcRmSleep", ::sleep(FAIL_RWC_INT("rwcRmSleep")));

	try {
		FileHandler *handler = getFileHandler(fd, (const char *) 0);

		if (handler != (FileHandler *) 0)
		{
			handler->rm();
		}
	} catch (rws::Exception &e) {
		throw Service::ServiceException(e.getStatus().getErrno(), e.what());
	} catch (exception &e) {
		throw Service::ServiceException(0, e.what());
	} catch ( ... ) {
		throw Service::ServiceException(0, __PRETTY_FUNCTION__);
	}
}


void
ReadWriteController::log(const char *msg, ssize_t len)
{
	if (msg != (const char *) 0)
	{
		len = ::strlen(msg);
	}

	if ((len > 0) && (logger_ != (DestFile *) 0))
	{
		MutexGuard guard(logMtx_);
		struct timeval nowTV;
		struct tm nowTM;
		char buf[1024];

		(void) ::gettimeofday(&nowTV, (struct timezone *) 0);
		(void) ::localtime_r(&(nowTV.tv_sec), &nowTM);

		int sz = ::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S.", &nowTM);
		char *p = &buf[sz];
		
		size_t left = sizeof(buf) - (p - buf);
		(void) ::snprintf(p, left, "%6.6ld %s ", nowTV.tv_usec, sysInfo_->getHostname());
		logger_->put(buf);
		logger_->put(msg);

		if (msg[len - 1] != '\n')
		{
			logger_->put("\n");
		}
		logger_->flush();
	}
}


void
ReadWriteController::logVar(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	try {
		char buf[10240];
		ssize_t sz = (ssize_t) ::vsnprintf(buf, sizeof(buf), fmt, ap);

		if (sz < 0)
		{
			APPLOG_WARN("Failed to vsnprintf(%s)=%d", buf, errno);
		}
		else if (sz >= (ssize_t) sizeof(buf))
		{
			APPLOG_WARN("Failed to vsnprintf(%s); need %d vs actual %d", buf,
				sz, sizeof(buf));
		}
		else
		{
			log(buf, sz);
		}
		va_end(ap);
	} catch ( ...) {
		va_end(ap);
	}
}


void
ReadWriteController::pullStatus(Service::NVPairList &l)
{
	Counter ctr(semCtr_);
	Config cfg;

	FAIL_RWC_IF("rwcPullStatusSleep", ::sleep(FAIL_RWC_INT("rwcPullStatusSleep")));

	pullStatus(cfg);

	NVPairUtil::append(&l, cfg);
}


void
ReadWriteController::pullStatus(Config &cfg)
{
	sysInfo_->getInfo(cfg);
	procInfo_->getInfo(cfg);
	netInfo_->getInfo(cfg);
	diskInfo_->getInfo(cfg);

	cfg.addLong(RWS_INSTANCE_ID, procInfo_->getInstanceTime().tv_sec, true);

	//@@TODO: Add more stuff.
}


void
ReadWriteController::reconfigure(const Service::NVPairList &cfg)
{
	Counter ctr(semCtr_);

	FAIL_RWC_IF("rwcReconfigureSleep", ::sleep(FAIL_RWC_INT("rwcReconfigureSleep")));

	int sz = cfg.length();

	// @@TODO: Should we do this?
	//
	cfg_.clear();

	for (int i = 0; i < sz; i++) {
		const Service::NVPair &p = cfg[i];
		const char *nm = p.name;
		const char *val = p.value;

		cfg_.addString(nm, val, true);

		APPLOG_DBG("%s=%s", nm, val);
	}
}


clusterio::length_t
ReadWriteController::read(const clusterio::open_info &fd,
	clusterio::length_t first_byte,
	clusterio::length_t size,
	clusterio::file_data_t_out data)
throw (CORBA::SystemException, clusterio::ClusterIOException, Service::ServiceException)
{
	Counter ctr(semCtr_);
	clusterio::length_t retVal;
	clusterio::file_data_t *d = new clusterio::file_data_t();
	clusterio::ClusterIOException cie;

	FAIL_RWC_IF("rwcReadSleep", ::sleep(FAIL_RWC_INT("rwcReadSleep")));

	try {
		verifyOpenFlags(fd, clusterio::o_rdonly);

		FileHandler *handler = getFileHandler(fd.handle);

		if (handler != (FileHandler *) 0)
		{
			d->length(size);

			CORBA::Octet *buf2 = d->get_buffer();
			char *buf = reinterpret_cast<char *>(buf2);
			FileIO::ReadContext ctxt(buf, size, false);

			APPLOG_DBG("Going to read %ld bytes", (long) size);

			retVal = handler->start_read(ctxt, first_byte, size);
			retVal = handler->finish_read(ctxt, first_byte);

			APPLOG_DBG("actually read %ld bytes", (long) retVal);

			if (retVal != size)
			{
				d->length(retVal);
			}
			data.ptr() = d;
		}
	} catch (rws::OpenException &oe) {
		d->length(0);
		data.ptr() = d;
		APPLOG_ALARM("oe: %s\n", oe.what());
		mapException(oe, cie);
		throw cie;
	} catch (rws::ReadException &re) {
		d->length(0);
		data.ptr() = d;
		APPLOG_ALARM("re: %s\n", re.what());
		mapException(re, cie);
		throw cie;
	} catch (rws::FileModeException &fme) {
		APPLOG_ALARM("fme: %s\n", fme.what());
		mapException(fme, cie);
		throw cie;
	} catch (exception &e) {
		d->length(0);
		data.ptr() = d;
		APPLOG_ALARM("Caught an exception %s", e.what());
		mapException(&e, cie, __PRETTY_FUNCTION__);
		throw cie;
	} catch ( ... ) {
		d->length(0);
		data.ptr() = d;
		APPLOG_ALARM("Caught something");
		mapException((const std::exception *) 0, cie, __PRETTY_FUNCTION__);
		throw cie;
	}

	return retVal;
}


#ifdef ALLOW_FALSE_FAILURES
void
ReadWriteController::setFailureConfig(const Config &cfg)
{
	forcedFailure_.clear();
	forcedFailure_ = cfg;
	char_ptr_vec *k = forcedFailure_.getKeys();
	for (char_ptr_vec::iterator itor = k->begin(); itor != k->end(); itor++) {
		const char *key = (*itor).get();
		const char *val = forcedFailure_.getString(key);

		APPLOG_DBG("forceFailure %s=%s", key, val);
	}
}
#endif


void
ReadWriteController::dumpState(const char *what,
	clusterio::ReadWriteService::dump_format fmt,
	clusterio::ReadWriteService::detail_level lvl, ostream &oss)
{
	bool all = (::strcmp(what, "all") == 0);

	if ((::strcmp(what, "rwc") == 0) || all)
	{
		if (fmt == clusterio::ReadWriteService::html)
		{
			oss << "<table>\n";
			oss << "<tr><td>logFile</td><td>" << logName_.get() << "</td></tr>\n";
			oss << "<tr><td>baseDir</td><td>" << baseDir_.get() << "</td></tr>\n";
			oss << "<tr><td>nic</td><td>" << nic_.get() << "</td></tr>\n";
			oss << "<tr><td>currentUserCount</td><td>" << getCurrentUserCount() << "</td></tr>\n";
			oss << "<tr><td>cacheSize</td><td>" << fhCache_->getCurrentSize() << "/"
				<< fhCache_->getMaxSize() << "</td></tr>\n";
			oss << "</table>\n";
		}
		else
		{
			oss << "logFile=" << logName_.get() << "\n";
			oss << "baseDir=" << baseDir_.get() << "\n";
			oss << "nic=" << nic_.get() << "\n";
			oss << "currentUserCount=" << getCurrentUserCount() << "\n";
			oss << "cacheSize=" << fhCache_->getCurrentSize() << "/"
				<< fhCache_->getMaxSize() << "\n";
		}
	}

	if (all)
	{
		long sz;
		char_ptr *keys = fhCache_->getAllKeys(sz);

		for (long i = 0; i < sz; i++)
		{
			FileHandler *fh = fhCache_->get(keys[i].get());

			if (fh != (FileHandler *) 0)
			{
				fh->dumpState(what, fmt, lvl, oss);
			}
		}
	}
	else
	{
		FileHandler *fh = fhCache_->get(what);

		if (fh != (FileHandler *) 0)
		{
			fh->dumpState(what, fmt, lvl, oss);
		}
	}

}

