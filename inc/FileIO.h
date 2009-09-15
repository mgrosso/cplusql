
#ifndef FILEIO_H
#define FILEIO_H 1

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <aio.h>
#include <list>

#include "Exclusion.h"


class FileHandler;
class ServerSocketReader;

class FileIO {
friend class FileHandler;
public:

    static const char *defaultSuffix;


	class AppendContext {
	friend class FileIO;
	public:
		inline size_t getOffset() const { return currSz; };
		inline size_t getLen() const { return writeB.aio_nbytes; };
	private:
		struct aiocb writeB;
		size_t currSz;
	};


	class ReadContext {
	friend class FileIO;
	public:
		inline ReadContext();
		inline ReadContext(char *buf, size_t len, bool ownBuf);
		inline ~ReadContext();

		inline char *takeBuffer();
		inline char *shareBuffer();
		inline size_t getActualLen();
		inline size_t getRequestedLen();
	private:
		std::list<aiocb> readB;
		bool ownBuf_;
		char *buf_;
		size_t actLen_;
		size_t reqLen_;
	};


	FileIO(FileHandler &fh, const char *basename,
		const char *suffix = FileIO::defaultSuffix, bool useAio = true);
	~FileIO();

	void init(int mode = 0755) throw ();

	inline const char *getFilename() const;

	void start_append(const char *, size_t len, AppendContext &) throw (rws::OpenException, rws::WriteException);
	bool finish_append(AppendContext &) throw (rws::Exception);

	void start_read(ReadContext &, size_t readStart, size_t len, size_t buffOffset) throw (rws::OpenException, rws::ReadException);
	void finish_read(ReadContext &) throw (rws::Exception);

	void rm() throw (rws::CloseException, rws::UnlinkException);

	ServerSocketReader *copyFrom(const char *host) throw (rws::Exception);
	ssize_t copyTo(const char *host, int port) throw (rws::Exception);

	void makeUnreachable() throw (rws::FileModeException, rws::CloseException);
	bool isPresent() throw ();

private:

	FileHandler &fh_;
	char *fname_;
	int dirMode_;
	int fdRd_;
	int fdWr_;
	size_t sz_;
	unsigned long currSeqNum_;
	bool useAio_;

	Mutex mtx_;


	FileIO(const FileIO &);

	void openWriter(int flags = O_WRONLY | O_CREAT | O_APPEND | O_DSYNC,
		int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWOTH) throw (rws::OpenException);
	void closeWriter() throw (rws::CloseException);

	void openReader(int flags = O_RDONLY, int mode = 0) throw (rws::OpenException);
	void closeReader() throw (rws::CloseException);

	void fire_read(char *buf, size_t offset, size_t len, ReadContext &ctxt) throw (rws::ReadException);

	size_t readBytes(int fd, char *buf, size_t offset, size_t len);
	size_t writeBytes(int fd, const char *buf, size_t len);
};


const char *
FileIO::getFilename() const
{
	return fname_;
};


FileIO::ReadContext::ReadContext()
	: ownBuf_(false), buf_((char *) 0), actLen_(0), reqLen_(0)
{
};


FileIO::ReadContext::ReadContext(char *buf, size_t len, bool ownBuf)
	: ownBuf_(ownBuf), buf_(buf), actLen_(0), reqLen_(len)
{
}


FileIO::ReadContext::~ReadContext()
{
	if (buf_ != (char *) 0)
	{
		if (ownBuf_)
		{
			delete [] buf_;
		}
		buf_ = (char *) 0;
	}
	ownBuf_ = false;
}


char *
FileIO::ReadContext::takeBuffer()
{
	char *retVal = buf_;

	buf_ = (char *) 0;
	ownBuf_ = false;

	return retVal;
}


char *
FileIO::ReadContext::shareBuffer()
{
	return buf_;
}


size_t
FileIO::ReadContext::getRequestedLen()
{
	return reqLen_;
}


size_t
FileIO::ReadContext::getActualLen()
{
	return actLen_;
}

#ifdef ALLOW_FALSE_FAILURES
#define FAIL_FI_IF(str,x) if (SHOULD_FAIL(fh_.getController().getFailure(), str)) { try { x ; } catch (...) { }; }
#define FAIL_FI_IF_THROW(str,x) if (SHOULD_FAIL(fh_.getController().getFailure(), str)) { x ; }
#define FAIL_FI_INT(str) fh_.getController().getFailure().getInt(str)
#define FAIL_FI_BOOL(str) fh_.getController().getFailure().getBool(str)
#define FAIL_FI_STR(str) fh_.getController().getFailure().getString(str)

#else

#define FAIL_FI_IF(str,x) ;
#define FAIL_FI_IF_THROW(str,x) ;
#define FAIL_FI_INT(str) 0
#define FAIL_FI_BOOL(str) false
#define FAIL_FI_STR(str) 0

#endif

#endif
