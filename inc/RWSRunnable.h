
#ifndef RWSRUNNABLE_H
#define RWSRUNABBLE_H 1

#include "clusterioC.h"
#include "Runnable.h"
#include "serviceC.h"


/**
These classes are for implementing chaining ReadWriteService operations
on to secondary servers, all done in a separate thread.
*/


class Mutex;
class ReadWriteController;


class RWSRunnable: public Runnable {
friend class ReadWriteController;
public:
	inline RWSRunnable(ReadWriteController &rwc, Mutex &, clusterio::ClusterIOException &, const Service::ServiceRef &, const clusterio::writer_id &);
	inline virtual ~RWSRunnable();

protected:
	ReadWriteController &rwc_;
	Mutex &mtx_;
	clusterio::ClusterIOException &cie_;
	const Service::ServiceRef &ref_;
	const clusterio::writer_id &wid_;
	struct timeval startT_;
	struct timeval endT_;

	void handleException(clusterio::ClusterIOException &);
	void handleException(Service::ServiceException &);
	void handleException(CORBA::SystemException &);
	void handleException(std::exception &);
	void handleException(void);

private:
	// Not implemented:
	RWSRunnable();
	RWSRunnable(const RWSRunnable &);
	RWSRunnable &operator=(const RWSRunnable &);
};


class AppendRunnable: public RWSRunnable {
friend class ReadWriteController;
public:
	AppendRunnable(ReadWriteController &rwc, clusterio::ReadWriteService_ptr rws, Mutex &, clusterio::ClusterIOException &, const Service::ServiceRef &, const clusterio::file_handle &, const clusterio::blockseq &blocks, const clusterio::writer_id &wid, CORBA::ULong seqNum);
	inline virtual ~AppendRunnable();

	virtual void run();

private:
	clusterio::ReadWriteService_ptr rws_;
	const clusterio::file_handle &handle_;
	const clusterio::blockseq &blocks_;
	CORBA::ULong seqNum_;

    // Not implemented:
	AppendRunnable();
	AppendRunnable(const AppendRunnable &);
	AppendRunnable &operator=(const AppendRunnable &);
};


class UndoRunnable: public RWSRunnable {
friend class ReadWriteController;
public:
	UndoRunnable(ReadWriteController &rwc, clusterio::ReadWriteService_ptr rws, Mutex &, clusterio::ClusterIOException &, const Service::ServiceRef &, const clusterio::file_handle &, const clusterio::writer_id &);
	inline virtual ~UndoRunnable();

	virtual void run();

	inline CORBA::ULong getNextSeqNum() const throw ();
	inline clusterio::length_t getBytesUndone() const throw ();

private:
	clusterio::ReadWriteService_ptr rws_;
	const clusterio::file_handle &handle_;
	CORBA::ULong seqNum_;
	clusterio::length_t bytesUndone_;

    // Not implemented:
	UndoRunnable();
	UndoRunnable(const UndoRunnable &);
	UndoRunnable &operator=(const UndoRunnable &);
};


class IORunnable : public Runnable {
public:
	inline IORunnable(ReadWriteController &, Mutex &);
	inline virtual ~IORunnable();

	size_t getBytesRead();
	size_t getBytesWritten();

	inline const Service::ServiceException &getSavedException() const throw ();

protected:
	ReadWriteController &rwc_;
	Mutex &mtx_;
	Service::ServiceException se_;
	size_t numRd_;
	size_t numWr_;
	struct timeval startT_;
	struct timeval endT_;

private:

    // Not implemented:
	IORunnable();
	IORunnable(const IORunnable &);
	IORunnable &operator=(const IORunnable &);
};


class ServerSocketReader: public IORunnable {
public:
	inline ServerSocketReader(ReadWriteController &rwc, Mutex &, const char *host, int toFd, ssize_t blkSz);
	virtual ~ServerSocketReader();

	virtual void run();

	inline int getServerPort() const throw ();
	inline const char *getHost() const throw ();

private:
	const char *host_;
	int port_;
	int serverSock_;
	int inFd_;

	int toFd_;
	ssize_t blkSz_;

	// Not implemented:
	ServerSocketReader();
	ServerSocketReader(const ServerSocketReader &);
	ServerSocketReader &operator=(const ServerSocketReader &);
};


RWSRunnable::RWSRunnable(ReadWriteController &rwc, Mutex &mtx, clusterio::ClusterIOException &cie, const Service::ServiceRef &ref, const clusterio::writer_id &wid)
	: rwc_(rwc), mtx_(mtx), cie_(cie), ref_(ref), wid_(wid)
{
};


RWSRunnable::~RWSRunnable()
{
};


AppendRunnable::~AppendRunnable()
{
};


UndoRunnable::~UndoRunnable()
{
};


CORBA::ULong
UndoRunnable::getNextSeqNum() const throw ()
{
	return seqNum_;
};


clusterio::length_t
UndoRunnable::getBytesUndone() const throw ()
{
	return bytesUndone_;
};


IORunnable::IORunnable(ReadWriteController &rwc, Mutex &mtx)
	: rwc_(rwc), mtx_(mtx)
{
};


IORunnable::~IORunnable()
{
};



const Service::ServiceException &
IORunnable::getSavedException() const throw ()
{
	return se_;
};


ServerSocketReader::ServerSocketReader(ReadWriteController &rwc, Mutex &mtx, const char *host, int toFd, ssize_t blkSz)
	: IORunnable(rwc, mtx), host_(host), toFd_(toFd), blkSz_(blkSz)
{
};


const char *
ServerSocketReader::getHost() const throw ()
{
	return host_;
};


int
ServerSocketReader::getServerPort() const throw ()
{
	return port_;
};

#endif
