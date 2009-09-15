
#ifndef READWRITECONTROLLER_H
#define READWRITECONTROLLER_H 1

#include <stdarg.h>
#include <sys/types.h>
#include <sys/statvfs.h>

#include "tao/Sequence_T.h"
#include "tao/Seq_Var_T.h"

#include "clusterioC.h"
#include "Config.h"
#include "Exclusion.h"
#include "FileHandler.h"
#include "LRUCache.h"
#include "PtrSemaphore.h"


class DestFile;
class DiskInfo;
class NetInfo;
class SystemInfo;
class ProcessInfo;
class RWSRunnable;

class ReadWriteController {
friend class RWSRunnable;
public:
	enum DumpFormat { html, kvpair };

	ReadWriteController(const Config &cfg);
	~ReadWriteController();

	void init();

	// IDL delegates, start
	void append(const clusterio::open_info & fd,
		const clusterio::blockseq & blocks,
		const clusterio::writer_id & writer,
		CORBA::ULong sequence_num)
	throw (clusterio::ClusterIOException, Service::ServiceException);

	void cp(const clusterio::file_handle & fd, const Service::ServiceRef & source, const clusterio::file_handle & sourcefd)
	throw (clusterio::ClusterIOException, Service::ServiceException);

	void cp(const clusterio::file_handle &fd, const char *destHost,
	    CORBA::Long destFilePort, CORBA::Long destAuditPort)
	throw (Service::ServiceException);


	void pullStatus(Service::NVPairList &);
	void pullStatus(Config &);

	void reconfigure(const Service::NVPairList &cfg);
	void dumpState(const char *what, clusterio::ReadWriteService::dump_format,
	    clusterio::ReadWriteService::detail_level,
		ostream &out);

	clusterio::length_t read(const clusterio::open_info & fd,
		clusterio::length_t first_byte,
		clusterio::length_t size,
		clusterio::file_data_t_out data)
	throw (CORBA::SystemException, clusterio::ClusterIOException,
			Service::ServiceException);

	void rm(const clusterio::file_handle &fd)
	throw (Service::ServiceException);

	void setAppendToken(const clusterio::file_handle &fd, CORBA::Long token)
	throw (Service::ServiceException);

	clusterio::file_handle_status_seq *stat(const clusterio::file_handle_seq &fdset, CORBA::Boolean checksums)
	throw (CORBA::SystemException, Service::ServiceException);

	clusterio::length_t undo_appends(const clusterio::open_info &fd,
		const clusterio::writer_id &writer,
		CORBA::ULong_out sequence_num)
	throw (clusterio::ClusterIOException, Service::ServiceException);



	// IDL delegates, end


	char_ptr convertPrivateNameToPath(clusterio::length_t id);

	FileHandler *getFileHandler(const clusterio::file_handle &fd, const char *wid = (const char *) 0);
	FileHandler *getFileHandler(long id, const char *relPath);

	inline const char *getBaseDir() const;

	struct timeval &getStartTime();
	inline const Config &getConfig() const;
	inline DiskInfo *getDiskInfo() const;
	inline NetInfo *getNetInfo() const;
	inline ProcessInfo *getProcessInfo() const;
	inline SystemInfo *getSystemInfo() const;
	int getCurrentUserCount() const;

	int getCacheCurrentSize() const throw ();
	int getCacheMaxSize() const throw ();
	void setCacheMaxSize(int sz);

	void log(const char *, ssize_t len = -1);
	void logVar(const char *fmt, ...);

#ifdef ALLOW_FALSE_FAILURES
	Config forcedFailure_;
	inline Config &getFailure() { return forcedFailure_; };
	void setFailureConfig(const Config &cfg);
#else
	inline void setFailureConfig(const Config &) {};
#endif

private:

	class HandlerCache: public LRUCache<FileHandler> {
	public:
		inline HandlerCache(bool autoPrune = true);
		inline HandlerCache(int maxSize, bool autoPrune = true);

	protected:
		virtual bool isSafeToRemove(const FileHandler *) const;
	};

	void mapException(const rws::Exception &e, clusterio::ClusterIOException &ce) throw ();
	void mapException(const rws::Exception &e, clusterio::io_exception_struct &ies) throw ();
	void mapException(const std::exception *e, clusterio::ClusterIOException &ce, const char *where) throw ();
	void mapException(const std::exception *e, clusterio::io_exception_struct &ies, const char *where) throw ();

	char_ptr dupSequence(const TAO_Unbounded_Sequence<CORBA::Octet> &, size_t &len);
	char_ptr dupSequence(const CORBA::Octet *p, size_t len);

	void ReadWriteController::verifyOpenFlags(const clusterio::open_info &fd,
		unsigned long req) throw (rws::FileModeException);

	struct timeval startT_;

	Config cfg_;
	char_ptr baseDir_;

	char_ptr logName_;
	DestFile *logger_;
	char_ptr nic_;
	DiskInfo *diskInfo_;
	NetInfo *netInfo_;
	ProcessInfo *procInfo_;
	SystemInfo *sysInfo_;

	HandlerCache *fhCache_;
	mutable PtrSemaphore semCtr_;
	Mutex mtx_;
	Mutex logMtx_;
};


ReadWriteController::HandlerCache::HandlerCache(bool autoPrune) : LRUCache<FileHandler>(autoPrune)
{
};


ReadWriteController::HandlerCache::HandlerCache(int maxSize, bool autoPrune) : LRUCache<FileHandler>(maxSize, autoPrune)
{
};


const char *
ReadWriteController::getBaseDir() const
{
	return baseDir_.get();
};


const Config &
ReadWriteController::getConfig() const
{
	return cfg_;
};


DiskInfo *
ReadWriteController::getDiskInfo() const
{
	return diskInfo_;
};


NetInfo *
ReadWriteController::getNetInfo() const
{
	return netInfo_;
};


ProcessInfo *
ReadWriteController::getProcessInfo() const
{
	return procInfo_;
};

SystemInfo *
ReadWriteController::getSystemInfo() const
{
	return sysInfo_;
};


#ifdef ALLOW_FALSE_FAILURES

#define SHOULD_FAIL(cfg,str) (cfg.findString(str) != (const char *) 0)
#define FAIL_RWC_IF(str,x) if (SHOULD_FAIL(forcedFailure_,str)) { try { x ; } catch (...) { }; }
#define FAIL_RWC_IF_THROW(str,x) if (SHOULD_FAIL(forcedFailure_,str)) { x ; }

#define FAIL_RWC_INT(str) forcedFailure_.getInt(str)
#define FAIL_RWC_BOOL(str) forcedFailure_.getBool(str)
#define FAIL_RWC_STR(str) forcedFailure_.getString(str)

#define PASTE(x,y) PASTE2(x,y)
#define PASTE2(x,y) x ## y

#else

#define SHOULD_FAIL(cfg,str) false

#define FAIL_RWC_IF(str,x) ;
#define FAIL_RWC_IF_THROW(str,x) ;
#define FAIL_RWC_INT(str) 0
#define FAIL_RWC_BOOL(str) false
#define FAIL_RWC_STR(str) 0

#endif

#endif
