
#ifdef __linux
#include <sys/sendfile.h>
#endif

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


const char *FileIO::defaultSuffix = ".f";

using namespace rws;

FileIO::FileIO(FileHandler &fh, const char *bName, const char *sfx,
	bool useAio)
	: fh_(fh), fname_(0), fdRd_(-1), fdWr_(-1), sz_(0), currSeqNum_(0),
		useAio_(useAio)
{
	const char *rootDir = fh_.getController().getBaseDir();

	fname_ = fwrap::mkFilePath(rootDir, bName, sfx);
}



FileIO::~FileIO()
{
	try {
		closeWriter();
	} catch (Exception &e) {
		APPLOG_WARN("Closing writer(%s) resulting in: %s", fname_, e.what());
	}
	try {
		closeReader();
	} catch (Exception &e) {
		APPLOG_WARN("Closing reader(%s) resulting in: %s", fname_, e.what());
	}

	if (fname_ != (char *) 0)
	{
		delete [] fname_;
		fname_ = (char *) 0;
	}
}


void
FileIO::init(int mode) throw ()
{
	dirMode_ = mode;
}


void
FileIO::openWriter(int flags, int mode) throw (OpenException)
{

	FAIL_FI_IF_THROW("fiOpenWriter", throw OpenException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiOpenWriter"))))

	if (fdWr_ == -1)
	{
		MutexGuard guard(mtx_);

		// double-check here, in case someone came through first.
		//
		if (fdWr_ == -1)
		{
			fdWr_ = fwrap::openfd(fname_, flags, mode, dirMode_);

			if (fdWr_ == -1)
			{
				throw OpenException(NICE_LOC, Resource(fname_), Status(errno));
			}
			sz_ = ::lseek(fdWr_, (off_t) 0, SEEK_END);
		}
	}
}


void
FileIO::closeWriter() throw (CloseException)
{
	FAIL_FI_IF_THROW("fiCloseWriter", throw CloseException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiCloseWriter"))))

	if (fdWr_ != -1)
	{
		int fd = fdWr_;
		int closeStat = ::close(fdWr_);

		fdWr_ = -1;
		if (closeStat != 0)
		{
			throw CloseException(NICE_LOC, Resource(fname_, fd), Status(errno));
		}
	}
}


void
FileIO::openReader(int flags, int mode) throw (OpenException)
{
	FAIL_FI_IF_THROW("fiOpenReader", throw OpenException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiOpenReader"))))

	if (fdRd_ == -1)
	{
		MutexGuard guard(mtx_);

		if (fdRd_ == -1)
		{
			// Don't call fwrap::openfd(), to avoid creating bogus/empty dirs.
			fdRd_ = ::open(fname_, flags, mode);

			if (fdRd_ == -1)
			{
				throw OpenException(NICE_LOC, Resource(fname_), Status(errno));
			}
		}
	}
}


void
FileIO::closeReader() throw (CloseException)
{
	FAIL_FI_IF_THROW("fiCloseReader", throw CloseException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiCloseReader"))))

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
FileIO::start_append(const char *s, size_t len, AppendContext &ctxt) throw (OpenException, WriteException)
{
	openWriter();

	FAIL_FI_IF_THROW("fiStartAppend", throw WriteException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiStartAppend"))))

	MutexGuard guard(mtx_);

	if (useAio_)
	{
		ctxt.writeB.aio_buf = const_cast<char *>(s);
		ctxt.writeB.aio_nbytes = len;
		ctxt.writeB.aio_fildes = fdWr_;
		ctxt.writeB.aio_offset = 0;
		ctxt.writeB.aio_reqprio = 0;
		ctxt.writeB.aio_sigevent.sigev_notify = SIGEV_NONE;

		ctxt.currSz = sz_;
		int writeStat = ::aio_write(&(ctxt.writeB));

		//@@TODO What if the write fails?
		//
		sz_ += len;
		if (writeStat == -1)
		{
			throw WriteException(NICE_LOC, Resource(fname_, fdWr_), Args(ctxt.currSz, len, -1), Status(errno, "aio_write"));
		}
	}
	else
	{
		ctxt.currSz = sz_;
		size_t numWr = writeBytes(fdWr_, const_cast<char *>(s), len);

		sz_ += numWr;
		if (numWr != len)
		{
			throw WriteException(NICE_LOC, Resource(fname_, fdWr_), Args(ctxt.currSz, len, numWr), Status(errno));
		}
	}
}


bool
FileIO::finish_append(AppendContext &ctxt) throw (rws::Exception)
{
	bool retVal = true;

	FAIL_FI_IF_THROW("fiFinishAppend", throw WriteException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiFinishAppend"))))

	if (useAio_)
	{
		MutexGuard guard(mtx_);
		const struct aiocb *aio[1] = { &(ctxt.writeB) };
		int suspendStat;

		do {
			suspendStat = ::aio_suspend(aio, 1, (const timespec *) 0);
		} while ((suspendStat == -1) && (errno == EINTR));

		retVal = (suspendStat == 0);
	}

	return retVal;
}


void
FileIO::fire_read(char *buf, size_t offset, size_t len, ReadContext &ctxt)
	throw (ReadException)
{
	if (useAio_)
	{
		struct aiocb blk;

		blk.aio_fildes = fdRd_;
		blk.aio_buf = buf;
		blk.aio_offset = offset;
		blk.aio_nbytes = len;
		blk.aio_reqprio = 0;
		blk.aio_sigevent.sigev_notify = SIGEV_NONE;
		ctxt.readB.insert(ctxt.readB.end(), blk);

		int rdStat = ::aio_read(&(ctxt.readB.back()));

		if (rdStat != 0)
		{
			throw ReadException(NICE_LOC, Resource(fname_, fdRd_), Args(offset, len, rdStat), Status(errno, "aio_read"));
		}
	}
	else
	{
		size_t numRd = readBytes(fdRd_, buf, offset, len);

		if (numRd != len)
		{
			throw ReadException(NICE_LOC, Resource(fname_, fdRd_), Args(offset, len, numRd), Status(errno));
		}
	}
}


size_t
FileIO::writeBytes(int fd, const char *buf, size_t len)
{
	size_t retVal = 0;
	ssize_t numWr;

	while ((numWr = ::write(fd, buf, len)) > 0)
	{
		if (numWr == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			break;
		}
		buf += numWr;
		len -= numWr;
		retVal += numWr;
	}
	
	return retVal;
}


size_t
FileIO::readBytes(int fd, char *buf, size_t offset, size_t len)
{
	size_t retVal = 0;

	while (len > 0)
	{
		ssize_t numRd = ::pread(fd, buf, len, offset);

		if (numRd == -1) {
			if (errno == EINTR)
			{
				continue;
			}
			break;
		}
		else if (numRd == 0)
		{
			break;
		}
		offset += numRd;
		buf += numRd;
		len -= numRd;
		retVal += numRd;
	}

	return retVal;
}


void
FileIO::start_read(ReadContext &ctxt, size_t readStart, size_t len, size_t bufOffset) throw (OpenException, ReadException)
{
	openReader();

	FAIL_FI_IF_THROW("fiStartRead", throw ReadException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiStartRead"))))

	if (ctxt.buf_ == (char *) 0)
	{
		ctxt.buf_ = new char[len + 1];
		ctxt.reqLen_ = len;
		bufOffset = 0;
	}

	char *currBuf = ctxt.buf_ + bufOffset;

	if (len > 0) {
		fire_read(currBuf, readStart, len, ctxt);
	}
}


void
FileIO::finish_read(ReadContext &ctxt) throw (Exception)
{
	FAIL_FI_IF_THROW("fiFinishRead", throw ReadException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiFinishRead"))))

	if (useAio_)
	{
		std::list<aiocb>::iterator itor = ctxt.readB.begin();
		int cnt = 0;

		while (itor != ctxt.readB.end())
		{
			struct aiocb *cb = &(*itor);
			int suspendStat;

			do {
				suspendStat = ::aio_suspend(&cb, 1, (const timespec *) 0);

				if (suspendStat == 0)
				{
					int szRd = aio_return(cb);

					if (szRd >= 0)
					{
						ctxt.actLen_ += szRd;
					}
					else
					{
						throw ReadException(NICE_LOC, Resource(fname_, cb->aio_fildes), Args(cb->aio_offset, cb->aio_nbytes, szRd), Status(errno, "aio_return"));
					}
				}
			} while ((suspendStat == -1) && (errno == EINTR));

			if (suspendStat == -1) {
				throw Exception(NICE_LOC, Resource(fname_, cb->aio_fildes), Args(cb->aio_offset, cb->aio_nbytes, -1), Status(errno, "aio_suspend"));
			}
			itor++;
			cnt++;
		}
	}
}


void
FileIO::rm() throw (CloseException, UnlinkException)
{
	MutexGuard guard(mtx_);

	FAIL_FI_IF_THROW("fiRm", throw UnlinkException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiRm"))))

	closeWriter();
	closeReader();

	int unStat = ::unlink(fname_);
    
	if (unStat != 0)
	{
		throw UnlinkException(NICE_LOC, Resource(fname_), Status(errno));
	}
}


void
FileIO::makeUnreachable() throw (FileModeException, CloseException)
{
	FAIL_FI_IF_THROW("fiMakeUnreachable", throw FileModeException(NICE_LOC, Resource(fname_), Status(FAIL_FI_INT("fiMakeUnreachable"))))

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
FileIO::isPresent() throw ()
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
FileIO::copyFrom(const char *host) throw (rws::Exception)
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
FileIO::copyTo(const char *host, int port) throw (rws::Exception)
{
	ssize_t retVal = 0;

	openReader();

	APPLOG_DBG("Connecting to server %s:%d", host, port);
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
				throw CopyException(NICE_LOC, Resource(fname_, sock), Args(off, len, -1), Status(errno, "sendfile"));
			}
			else if (retVal != slen)
			{
				throw CopyException(NICE_LOC, Resource(fname_, sock), Args(off, len, retVal), Status(errno, "sendfile didn't write all the bytes"));
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
