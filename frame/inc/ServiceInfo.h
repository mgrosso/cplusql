
#ifndef SERVICEINFO_H
#define SERVICEINFO_H 1

#include <netinet/in.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <stdarg.h>
#include <unistd.h>

class Config;


//
// Really just a place to factor out common util code into.
//

class ServiceInfo {
public:
	inline ServiceInfo() throw () {};
	inline virtual ~ServiceInfo() {};

	virtual void getInfo(Config &) = 0;

protected:
	// Get the name of the file that symlink is pointing to.  Zero-length
	// returned means something bad/bogus happened.
	char_ptr readSymlink(const char *symlink) const;

	static void addLong(Config &cfg, const char *nm, long long val);
	static void addDouble(Config &cfg, const char *nm, double val);
};


class DiskInfo: public ServiceInfo {
public:

	// From http://www.redhat.com/archives/fedora-devel-list/2003-November/msg01010.html
	//
	//
	// WARNING: These are highly linux 2.6 dependent!
	//
	//

	struct DiskStats {
		unsigned long rd_compl;
		unsigned long rd_merg;
		unsigned long rd_sectors;
		unsigned long rd_ms;
		unsigned long wr_compl;
		unsigned long wr_merg;
		unsigned long wr_sectors;
		unsigned long wr_ms;
		unsigned long io_curr;
		unsigned long io_ms;
		unsigned long io_weight;
	};

	struct PartitionStats {
		unsigned long rd_compl;
		unsigned long rd_sectors;
		unsigned long wr_compl;
		unsigned long wr_sectors;
	};

	DiskInfo(const char *baseDir);
	DiskInfo(char_ptr baseDir);
	virtual ~DiskInfo();

	inline const char *getBaseDir() const throw ();
	inline const char *getTrueDir() const throw ();

	// Native/optimum file-system transfer block size.  Based upon the
	// filesystem that baseDir is mount on.
	long getFsXferBlockSize();

	inline const char *getDevicePartition() const;

	bool getDiskStats(DiskStats &ds);
	bool getPartitionStats(PartitionStats &ps);

	void getInfo(Config &);

	const struct statvfs &getFsStats();
private:
	// Private so we can't use them.
	//
	DiskInfo(const DiskInfo &);
	DiskInfo &operator=(const DiskInfo &);

	void init() throw ();
	int loadFsStats();
	char_ptr findPartition();

	char_ptr baseDir_;
	char_ptr trueDir_;
	char_ptr devicePartition_;

	struct statvfs fsInfo_;
};


class NetInfo: public ServiceInfo {
public:

	struct IfStats {
		long long recv_bytes;
		long long recv_pkt;
		long long recv_err;
		long long recv_drop;
		long long recv_fifo;
		long long recv_frame;
		long long recv_compr;
		long long recv_mcast;

		long long tran_bytes;
		long long tran_pkt;
		long long tran_err;
		long long tran_drop;
		long long tran_fifo;
		long long tran_colls;
		long long tran_carr;
		long long tran_compr;
	};

	NetInfo(const char *nic);
	NetInfo(char_ptr nic);
	virtual ~NetInfo();

	inline const char *getNic() const throw ();
	inline in_addr_t getIpAddr() const throw ();
	inline const char *getIpStr() const throw ();
	inline const char *getInterface() const throw ();

	bool getIfStats(IfStats &is);

	void getInfo(Config &);

private:
	char_ptr nic_;
	in_addr_t ipAddr_;
	char_ptr ipStr_;
	char_ptr if_;

	void init() throw ();

};


//
// Allows for retrieval of information about this process.
//

class ProcessInfo: public ServiceInfo {
public:
	ProcessInfo() throw ();
	virtual ~ProcessInfo() throw ();

	inline int getPid() const throw ();
	char_ptr getCmdline() const;
	char_ptr getCwd() const;
	char_ptr getExe() const;

	// Get time this process started
	//
	long getStartTime() const throw ();

	// Get time this object was created.
	//
	inline const struct timeval &getInstanceTime() const throw ();

	int getUsage(struct rusage *) throw ();

	// Change this processes max open files allowed.  If newMax is ok, 0
	// is returned; otherwise return errno.
	//
	int setMaxFD(int newMax) throw ();

	int getMaxFD() throw ();
	int getCurrentFDUsage() throw ();

	void getInfo(Config &);

private:
	// Private so we can't use them.
	ProcessInfo(const ProcessInfo &);
	ProcessInfo &operator=(const ProcessInfo &);
	int pid_;
	struct timeval startT_;
};



//
// Allows for retrieval of information about this system.
//

class SystemInfo: public ServiceInfo {
public:
	SystemInfo();
	virtual ~SystemInfo();

	// How long has the box been up.
	long getUptime() const;

	// Max number of open fd's the system will let a process have.
	long getMaxFD() const throw ();

	// Obvious.
	inline const char *getHostname() const throw ();

	//
	bool getLoadAvg(float &avg1, float &avg2, float &av3);

	void getInfo(Config &);

private:
	// Private so we can't use them.
	//
	SystemInfo(const SystemInfo &);
	SystemInfo &operator=(const SystemInfo &);

	void init() throw ();

	struct utsname uname_;
};

const char *
DiskInfo::getBaseDir() const throw ()
{
	return baseDir_.get();
};


const char *
DiskInfo::getDevicePartition() const
{
	return devicePartition_.get();
};

const char *
DiskInfo::getTrueDir() const throw ()
{
	return trueDir_.get();
};


const char *
NetInfo::getInterface() const throw ()
{
	return if_.get();
};


in_addr_t
NetInfo::getIpAddr() const throw ()
{
	return ipAddr_;
}

const char *
NetInfo::getIpStr() const throw ()
{
	return ipStr_.get();
};


const char *
NetInfo::getNic() const throw ()
{
	return nic_.get();
};

int
ProcessInfo::getPid() const throw ()
{
	return pid_;
};


const struct timeval &
ProcessInfo::getInstanceTime() const throw ()
{
	return startT_;
};

const char *
SystemInfo::getHostname() const throw ()
{
	return uname_.nodename;
};

#endif
