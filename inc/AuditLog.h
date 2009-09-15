
#ifndef AUDITLOG_H
#define AUTIDLOG_H 1

//#define ALLOW_FALSE_FAILURES 1

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <functional>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "Exclusion.h"
#include "RWSExceptions.h"
#include "clusterioC.h"


class FileHandler;
class ServerSocketReader;


class AuditLog {
public:
//	using namespace rws;
	static const char *defaultSuffix;

	enum RecordType { appendRecord = 'A', undoRecord = 'B' };

#pragma pack(2)
	struct Entry {
		inline Entry() throw ();
		inline Entry(const char *) throw ();

		size_t offset;
		size_t len;
		unsigned long seqNo;
		timeval startT;
		timeval endT;
		long long checksum[clusterio::clusterio_checksum_size];
		clusterio::e_checksum_type checksumType;
		RecordType type;
		char writerId[clusterio::writer_id_len];
		char nullTerm;

		class Writer_Eq: public unary_function<AuditLog::Entry, bool> {
		public:
			explicit Writer_Eq(const AuditLog::Entry &e) : e_(e) {};
			bool operator()(const AuditLog::Entry &x) const;

		private:
			const AuditLog::Entry &e_;
		};

		void copyFrom(const char *s) throw ();
		void copyFrom(const clusterio::writer_id &) throw ();
		void copyTo(clusterio::writer_id &) const throw ();
	};
#pragma pack(0)

	class ReadContext {
	friend class AuditLog;
	public:
		inline ReadContext() throw ();

		inline AuditLog::Entry &getEntry() throw ();

	private:
		
		size_t nextReadOffset_;
		Entry entry_;
	};


	AuditLog(FileHandler &fh, const char *baseName, const char *suffix = AuditLog::defaultSuffix);
	~AuditLog();
	
	void init(int dirMode = 0755) throw ();

	inline const char *getFilename() const;

	void write(Entry &e) throw (rws::OpenException, rws::WriteException);

	void openReader(int flags = O_RDONLY, int mode = 0) throw (rws::OpenException);
	bool readNext(AuditLog::ReadContext &ctxt) throw (rws::OpenException, rws::ReadException);
	void closeReader() throw (rws::CloseException);
	void closeWriter() throw (rws::CloseException);

	bool isPresent() throw ();

	void rm() throw (rws::CloseException, rws::UnlinkException);

	ServerSocketReader *copyFrom(const char *host) throw (rws::Exception);
	ssize_t copyTo(const char *host, int port) throw (rws::Exception);

	void makeUnreachable() throw (rws::FileModeException, rws::CloseException);

private:

	FileHandler &fh_;
	char *fname_;
	int dirMode_;
	int fdRd_;
	int fdWr_;
	Mutex mtx_;

	void openWriter(int flags = O_WRONLY | O_CREAT | O_APPEND | O_DSYNC,
		int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWOTH) throw (rws::OpenException);

	// Return # of bytes written, or -1 on error.
	//
	ssize_t writeBytes(int fd, const char *p, size_t len);

	// Return # of bytes written, or -1 on error.  EOF indicated by 0.
	//
	ssize_t readBytes(int fd, char *p, size_t offset, size_t len);
};




const char *
AuditLog::getFilename() const
{
	return fname_;
};


AuditLog::Entry::Entry() throw ()
{
	(void) ::memset(this, '\0', sizeof(AuditLog::Entry));
};


AuditLog::Entry::Entry(const char *s) throw ()
{
	(void) ::memset(this, '\0', sizeof(AuditLog::Entry));
	strncpy(writerId, s, sizeof(writerId));
};


AuditLog::Entry &
AuditLog::ReadContext::getEntry() throw ()
{
	return entry_;
};


AuditLog::ReadContext::ReadContext() throw () : nextReadOffset_(0)
{
};


#ifdef ALLOW_FALSE_FAILURES
#define FAIL_AL_IF(str,x) if (SHOULD_FAIL(fh_.getController().getFailure(), str)) { try { x ; } catch (...) { }; }
#define FAIL_AL_IF_THROW(str,x) if (SHOULD_FAIL(fh_.getController().getFailure(), str)) { x ; }

#define FAIL_AL_INT(str) fh_.getController().getFailure().getInt(str)
#define FAIL_AL_BOOL(str) fh_.getController().getFailure().getBool(str)
#define FAIL_AL_STR(str) fh_.getController().getFailure().getString(str)

#else

#define FAIL_AL_IF(str,x) ;
#define FAIL_AL_IF_THROW(str,x) ;
#define FAIL_AL_INT(str) 0
#define FAIL_AL_BOOL(str) false
#define FAIL_AL_STR(str) 0

#endif

#endif
