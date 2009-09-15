
#ifndef FILEHANDLER_H
#define FILEHANDLER_H 1

#define NEED_HASH_MAP
#include "frame_config.h"

#include <ext/hash_map>

#include <aio.h>
#include <list>
#include <map>
#include <string.h>
#include <vector>


#include "AppLog.h"
#include "AuditLog.h"
#include "clusterioC.h"
#include "CRC48HashAlg.h"
#include "DLList.h"
#include "Exclusion.h"
#include "FileIO.h"
#include "LRUCache.h"
#include "ptr_strdup.h"


class ReadWriteController;

class FileHandler {
//	using namespace std;
public:

	FileHandler(ReadWriteController &rwc, clusterio::length_t id, const char *filename, const char *aSuffix = AuditLog::defaultSuffix, const char *fSuffix = FileIO::defaultSuffix);
	~FileHandler();

	void init();

	clusterio::length_t undo_appends(const char *writerId, CORBA::ULong &nextSeq)
		throw (rws::OpenException, rws::WriteException, rws::BadDataException);

	void append(const clusterio::blockseq &blocks, const char *id, size_t seqNum)
		throw (rws::OpenException, rws::WriteException, rws::BadDataException);
	
	void append(const char *writerId, unsigned long seqNo, const char *data, ssize_t len, clusterio::e_checksum_type type, const clusterio::checksum_t *sum) throw (rws::OpenException, rws::WriteException);

	size_t start_read(FileIO::ReadContext &ctxt, size_t offset, size_t len) throw (rws::OpenException, rws::ReadException);
	size_t finish_read(FileIO::ReadContext &ctxt, size_t offset) throw (rws::ReadException);

	clusterio::length_t undo(const char *writerId, size_t offset, size_t len) throw (rws::OpenException, rws::WriteException);

	void setAppendToken(long long token) throw (rws::BadDataException);
	inline long long getAppendToken() const throw ();
	
	void copyFrom(clusterio::ReadWriteService_ptr rws, const clusterio::file_handle &sourcefd, const char *nic) throw (rws::ThreadException);

	void copyTo(const char *destHost, int destFilePort, int destAuditPort)
		throw (rws::Exception);

	void rm() throw (rws::CloseException, rws::UnlinkException);

	inline AuditLog &getAuditLog() { return auditor_; };
	inline FileIO &getFileIO() { return fio_; };
	int getCurrentUserCount() const;
	inline ReadWriteController &getController() const throw ();
	inline clusterio::length_t getPrivateId() const throw ();

	void makeUnreachable() throw (rws::FileModeException, rws::CloseException);
	bool isPresent() throw ();

	inline const char *getFilename() const throw();

	list<AuditLog::Entry> *getValidEntries();
	void dumpState(const char *what,
	    clusterio::ReadWriteService::dump_format fmt,
	    clusterio::ReadWriteService::detail_level lvl, ostream &oss);


private:

	struct RecordAdj: public DLList::Node {
		inline RecordAdj(const AuditLog::Entry &, size_t adj) throw ();
		inline virtual ~RecordAdj() throw ();

		AuditLog::Entry entry;
		size_t logOffset;
		size_t pendingAdj;
	};

	typedef list<RecordAdj *> WriterAdj;

	struct WriterInfo {
		char_ptr wid;
		size_t seqNo;
		WriterAdj *recs;		
	};


	typedef hash_map<const char *, WriterInfo *, hash<const char *>, eqstr> WriterRecords;

	ReadWriteController &rwc_;
	clusterio::length_t privateId_;
	char_ptr fname_;
	AuditLog auditor_;
	FileIO fio_;
	Mutex mtx_;
	mutable PtrSemaphore semCtr_;
	long long appendToken_;

	enum InfoMode { None, Append, Read } mode_;
	DLList goodRecords_;
	WriterRecords widInfo_;
	size_t leadingAdj_;
	size_t trailingAdj_;
	bool needFixup_;
	

	// Make private so the compiler won't silently do it.
	FileHandler(const FileHandler &);
	FileHandler &operator=(const FileHandler &);

	WriterInfo *findWriterInfo(const char *wid);


	void loadAuditLogForAppend();
	void loadAuditLogForRead();

	// Return true if we've not seen this seqNo before.
	bool checkWriterSeqNo(const char *wid, size_t seqNo);
	bool checkWriterSeqNo(WriterInfo *, size_t seqNo);

	void verifyChecksum(const clusterio::block &blk, int blkNo) throw (rws::BadDataException);
	char *createFilePath(clusterio::file_handle &handle);
	clusterio::length_t adjRecords(const AuditLog::Entry &r);
	void fixupGoodList();

	void dump();
};


ReadWriteController &
FileHandler::getController() const throw ()
{
	return rwc_;
};


clusterio::length_t
FileHandler::getPrivateId() const throw ()
{
	return privateId_;
};


const char *
FileHandler::getFilename() const throw()
{
	return fname_.get();
};


FileHandler::RecordAdj::RecordAdj(const AuditLog::Entry &e, size_t adj) throw ()
	: entry(e), logOffset(e.offset - adj), pendingAdj(0)
{
	if (e.offset < adj)
	{
		logOffset = 0;
	}
};


FileHandler::RecordAdj::~RecordAdj() throw ()
{
};


long long
FileHandler::getAppendToken() const throw ()
{
	return appendToken_;
};


#ifdef ALLOW_FALSE_FAILURES

#define FAIL_FH_IF(str,x) if (SHOULD_FAIL(rwc_.getFailure(), str)) { try { x ; } catch (...) { }; }
#define FAIL_FH_IF_THROW(str,x) if (SHOULD_FAIL(rwc_.getFailure(), str)) { x ; }
#define FAIL_FH_INT(str) rwc_.getFailure().getInt(str)
#define FAIL_FH_BOOL(str) rwc_.getFailure().getBool(str)
#define FAIL_FH_STR(str) rwc_.getFailure().getString(str)

#else

#define FAIL_FH_IF(str,x) ;
#define FAIL_FH_IF_THROW(str,x) ;
#define FAIL_FH_INT(str) 0
#define FAIL_FH_BOOL(str) bool
#define FAIL_FH_STR(str) 0

#endif

#endif
