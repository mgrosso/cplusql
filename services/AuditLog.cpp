
#ifdef __linux
#include <sys/sendfile.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#include "AppLog.h"
#include "fwrap.h"
#include "FileHandler.h"
#include "netwrap.h"
#include "ReadWriteController.h"
#include "RWSRunnable.h"
#include "ServiceInfo.h"


const char *AuditLog::defaultSuffix = ".audit";

using namespace rws;



bool
AuditLog::Entry::Writer_Eq::operator()(const AuditLog::Entry &x) const
{
	int i = ::strncmp(e_.writerId, x.writerId, sizeof(x.writerId));
	bool retVal = (i == 0);

	return retVal;
}


void
AuditLog::Entry::copyFrom(const char *s) throw ()
{
	if (s == (const char *) 0)
	{
		::memset(writerId, '\0', sizeof(writerId));
	}
	else
	{
		::strncpy(writerId, s, sizeof(writerId));
	}
}


void
AuditLog::Entry::copyFrom(const clusterio::writer_id &wid) throw ()
{
	const CORBA::Octet *p = wid.get_buffer();

	copyFrom(reinterpret_cast<const char *>(p));
}


void
AuditLog::Entry::copyTo(clusterio::writer_id &wid) const throw ()
{
	size_t len = 0;

	for (len = 0; len < sizeof(writerId); len++)
	{
		if (writerId[len] == '\0')
		{
			break;
		}
	}

	wid.length(len);

	for (size_t i = 0; i < len; i++)
	{
		wid[i] = writerId[i];
	}
}


AuditLog::AuditLog(FileHandler &fh, const char *bName, const char *sfx)
	: fh_(fh), fname_((char *) 0), fdRd_(-1), fdWr_(-1)
{
	const char *rootDir = fh.getController().getBaseDir();

	fname_ = fwrap::mkFilePath(rootDir, bName, sfx);
}


AuditLog::~AuditLog()
{
	try {
		closeReader();
	} catch (CloseException &ce) {
		APPLOG_WARN("Closing reader(%s) resulting in: %s", fname_, ce.what());
	}

	try {
		closeWriter();
	} catch (CloseException &ce) {
		APPLOG_WARN("Closing writer(%s) resulting in: %s", fname_, ce.what());
	}

	if (fname_ != (char *) 0)
	{
		delete [] fname_;
		fname_ = (char *) 0;
	}
}


/*
@return true if the fd was closed.
*/

void
AuditLog::closeWriter() throw (CloseException)
{
    FAIL_AL_IF_THROW("alCloseWriter", throw CloseException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alCloseWriter"))))

	if (fdWr_ != -1)
	{
		int closeStat = ::close(fdWr_);
		int fd = fdWr_;

		fdWr_ = -1;
		if (closeStat != 0)
		{
			throw CloseException(NICE_LOC, Resource(fname_, fd), Status(errno));
		}
	}
}


/*
@return true if the fd was closed.
*/

void
AuditLog::closeReader() throw (CloseException)
{
    FAIL_AL_IF_THROW("alCloseReader", throw CloseException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alCloseReader"))))

	if (fdRd_ != -1)
	{
		int fd = fdRd_;
		int closeStat = ::close(fdRd_);

		fdRd_ = -1;
		if (closeStat != 0)
		{
			throw CloseException(NICE_LOC, Resource(fname_, fd), Status(errno));
		}
	}
}


void
AuditLog::init(int mode) throw ()
{
	dirMode_ = mode;
}


void
AuditLog::openWriter(int flags, int mode) throw (OpenException)
{
    FAIL_AL_IF_THROW("alOpenWriter", throw OpenException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alOpenWriter"))))

	if (fdWr_ == -1)
	{
		MutexGuard guard(mtx_);

		if (fdWr_ == -1)
		{
			fdWr_ = fwrap::openfd(fname_, flags, mode, dirMode_);
			if (fdWr_ == -1)
			{
				throw OpenException(NICE_LOC, Resource(fname_), Status(errno));
			}
		}
	}
}


void
AuditLog::openReader(int flags, int mode) throw (OpenException)
{
    FAIL_AL_IF_THROW("alOpenReader", throw OpenException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alOpenReader"))))

	if (fdRd_ == -1)
	{
		MutexGuard guard(mtx_);

		if (fdRd_ == -1)
		{
			// Don't call fwrap::openfd(), to avoid creating bogus/empty dirs.
			//
			fdRd_ = ::open(fname_, flags, mode);
			if (fdRd_ == -1)
			{
				throw OpenException(NICE_LOC, Resource(fname_), Status(errno));
			}
		}
	}
}


void
AuditLog::write(Entry &e) throw (OpenException, WriteException)
{
    FAIL_AL_IF("alWriteSleep", ::sleep(FAIL_AL_INT("alWriteSleep")))

    FAIL_AL_IF_THROW("alWrite", throw WriteException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alWrite"))))

	size_t sz = sizeof(Entry);
	struct timeval nowT;

	openWriter();
	(void) ::gettimeofday(&nowT, 0);

	if (e.startT.tv_sec == 0L)
	{
		e.startT = nowT;
	}
	e.endT = nowT;

	size_t numWr = writeBytes(fdWr_, reinterpret_cast< const char * >(&e), sz);

	if (numWr != sz)
	{
		throw WriteException(NICE_LOC, Resource(fname_, fdWr_), Args(-1, sz, numWr), Status(errno));
	}
}


bool
AuditLog::readNext(AuditLog::ReadContext &ctxt) throw (OpenException, ReadException)
{
    FAIL_AL_IF("alReadNextSleep", ::sleep(FAIL_AL_INT("alReadNextSleep")))

    FAIL_AL_IF_THROW("alReadNext", throw ReadException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alReadNext"))))

	bool retVal = true;
	AuditLog::Entry &entry = ctxt.getEntry();
	size_t sz = sizeof(entry);
	size_t offset = ctxt.nextReadOffset_;

	openReader();

	size_t numRd = readBytes(fdRd_, reinterpret_cast< char * >(&entry), offset, sz);

	ctxt.nextReadOffset_ += sz;

	if (numRd == 0)
	{
		retVal = false;
	}
	else if (numRd != sz)
	{
		throw ReadException(NICE_LOC, Resource(fname_, fdRd_), Args(offset, sz, numRd), Status(errno));
	}

	return retVal;
}


ssize_t
AuditLog::writeBytes(int fd, const char *p, size_t len)
{
	ssize_t retVal = 0;
	MutexGuard guard(mtx_);

	while (len > 0)
	{
		ssize_t numWr = ::write(fd, p, len);

		if (numWr == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			if (retVal == 0)
			{
				retVal = -1;
			}
			return retVal;
		}
		p += numWr;
		len -= numWr;
		retVal += numWr;
	}

	return retVal;
}


ssize_t
AuditLog::readBytes(int fd, char *p, size_t offset, size_t len)
{
	size_t retVal = 0;

	while (len > 0)
	{
		ssize_t numRd = ::pread(fd, p, len, offset);

		if (numRd == -1)
		{
			if (errno == EINTR)
			{
				//@@TODO Handle looping forever.
				continue;
			}
			return -1;
		} else if (numRd == 0)
		{
			break;
		}

		offset += numRd;
		p += numRd;
		len -= numRd;
		retVal += numRd;
	}

	return retVal;
}


void
AuditLog::rm() throw (CloseException, UnlinkException)
{
	MutexGuard guard(mtx_);

    FAIL_AL_IF_THROW("alRm", throw UnlinkException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alRm"))))

	closeReader();
	closeWriter();

	int unStat = unlink(fname_);

	if (unStat != 0)
	{
		throw UnlinkException(NICE_LOC, Resource(fname_), Status(errno));
	}
}


void
AuditLog::makeUnreachable() throw (FileModeException, CloseException)
{
	MutexGuard guard(mtx_);

    FAIL_AL_IF_THROW("alMakeUnreachable", throw FileModeException(NICE_LOC, Resource(fname_), Status(FAIL_AL_INT("alMakeUnreachable"))))

	// Otherwise known as chmod 000
	mode_t bits = S_IRWXU | S_IRWXG | S_IRWXO;
	int fd = max(fdWr_, fdRd_);
	int st;

	if (fd == -1)
	{
		st = fwrap::chmod(fname_, (mode_t) 0, bits);
	}
	else
	{
		st = fwrap::chmod(fd, (mode_t) 0, bits);
	}

	closeReader();
	closeWriter();

	if (st != 0)
	{
		throw FileModeException(NICE_LOC, Resource(fname_, fd), Status(st, "Failure doing chmod 000"));
	}
}


bool
AuditLog::isPresent() throw ()
{
	bool retVal = true;

	if ((fdWr_ == -1) && (fdRd_ == -1))
	{
		struct stat buf;
		int st = ::stat(fname_, &buf);

		retVal = (st == 0);
	}

	return retVal;
}


ServerSocketReader *
AuditLog::copyFrom(const char *host) throw (rws::Exception)
{
	ServerSocketReader *retVal;
	// Let's just pick 16, since that might be 16*4k = 64K, which sounds nice.
	//
	size_t blkSz = 16 * fh_.getController().getDiskInfo()->getFsXferBlockSize();
    
	closeWriter();
	openWriter(O_WRONLY | O_CREAT | O_APPEND | O_TRUNC);
    
	retVal = new ServerSocketReader(fh_.getController(), mtx_, host, fdWr_, blkSz);

	return retVal;
}


ssize_t
AuditLog::copyTo(const char *host, int port) throw (rws::Exception)
{
	ssize_t retVal = 0;

    openReader();

	bool useSendfile = true;
	int sock = netwrap::connectToServer(host, port);

	if (sock < 0)
	{
		throw CopyException(NICE_LOC, Resource(host, port), Args(true, fname_, fdRd_), Status(errno, "Failed to connect to server"));
	}

	try {
		FAIL_FI_IF_THROW("fiCp", throw CopyException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiCp"))))

#ifdef __linux
		if (useSendfile)
		{
			APPLOG_DBG("doing sendlocal");
			size_t len = ::lseek(fdRd_, 0, SEEK_END);
			ssize_t slen = len;

			(void) ::lseek(fdRd_, 0, SEEK_SET);

			off_t off = 0;
			retVal = ::sendfile(sock, fdRd_, &off, len);

			if (retVal == -1)
			{
				throw CopyException(NICE_LOC, Resource(fname_, fdRd_), Args(off, len, -1), Status(errno, "sendfile"));
			}
			else if (retVal != slen)
			{
				throw CopyException(NICE_LOC, Resource(fname_, fdRd_), Args(off, len, retVal), Status(errno, "sendfile write didn't write everything"));
			}
		}
		else
#endif
		{
			APPLOG_DBG("doing socket");
			size_t blkSz = fh_.getController().getDiskInfo()->getFsXferBlockSize();
			int numRd;
			char buf[blkSz];

			while ((numRd = fwrap::read(fdRd_, buf, blkSz)) > 0)
			{
				int numWr = fwrap::write(sock, buf, numRd);

				if (numWr != numRd)
				{
					throw CopyException(NICE_LOC, Resource(host, port), Args(retVal, numRd, numWr), Status(errno, "Failed to copyTo socket"));
				}
				retVal += numWr;
			}
			if (numRd == -1)
			{
				throw ReadException(NICE_LOC, Resource(fname_, fdRd_), Args(retVal, blkSz, numRd), Status(errno, "Failed to read from file"));
			}
		}
		netwrap::closeClient(sock);
		closeReader();
	} catch (...) {
		netwrap::closeClient(sock);
		closeReader();
		throw;
	}

	return retVal;
}
