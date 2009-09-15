
// From "man getifaddrs":
//   If both <net/if.h> and <ifaddrs.h> are being included, <net/if.h> must be
//   included before <ifaddrs.h>.

#include <arpa/inet.h>
#include <sys/socket.h>
#ifdef __linux
#include <linux/version.h>
#endif
#include <net/if.h>
#include <sys/poll.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>

// This is approximate.  x86_64 2.6.2 has it, while x86 2.4.7 doesn't.
//
#if LINUX_VERSION_CODE >= 132608
#include <ifaddrs.h>
#endif

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "AppLog.h"
#include "Config.h"
#include "fwrap.h"
#include "netwrap.h"
#include "ServiceInfo.h"
#include "Slurp.h"


char_ptr
ServiceInfo::readSymlink(const char *p) const
{
	char buf[1024];
	int len = ::readlink(p, buf, sizeof(buf) - 1);

	if (len < 0)
	{
		len = 0;
	}
	buf[len] = '\0';

	char_ptr retVal = ptr_strdup(buf);

	return retVal;
}


void
ServiceInfo::addLong(Config &cfg, const char *nm, long long val)
{
	char buf[128];

	(void) ::snprintf(buf, sizeof(buf), "%lld", val);

	cfg.addString(nm, buf, true);
}


void
ServiceInfo::addDouble(Config &cfg, const char *nm, double val)
{
	char buf[128];

	(void) ::snprintf(buf, sizeof(buf), "%.5f", val);

	cfg.addString(nm, buf, true);
};


//-----------------------------

DiskInfo::DiskInfo(const char *dir)
{
	if (dir != (const char *) 0)
	{
		baseDir_ = ptr_strdup(dir);
	}
	init();
}


DiskInfo::DiskInfo(char_ptr dir) : baseDir_(dir)
{
	init();
}


DiskInfo::~DiskInfo()
{
}


void
DiskInfo::init() throw ()
{
	char cwd[4096];
	bool ok = false;
	int currDir = ::open(".", O_RDONLY);

	if (currDir == -1)
	{
		APPLOG_WARN("Cannot open '.'; errno=%d", errno);
	}
	else if (baseDir_.get() == (char *) 0)
	{
		APPLOG_WARN("Null baseDir!");
	}
	else
	{
		if (::chdir(baseDir_.get()) == 0)
		{
			if (::getcwd(cwd, sizeof(cwd)) != (char *) 0)
			{
				ok = true;
			}
			else
			{
				APPLOG_WARN("Could not getwd for %s", baseDir_.get());
			}

			if (::fchdir(currDir) != 0)
			{
				APPLOG_ALARM("Couldn't fchdir back to where we started");
			}
		}
		else
		{
			APPLOG_WARN("Cannot chdir to %s", baseDir_.get());
		}
		(void) ::close(currDir);
	}

	if (ok)
	{
		trueDir_ = ptr_strdup(cwd);
	}
	else
	{
		APPLOG_WARN("Using directory of %s", baseDir_.get());
		trueDir_ = baseDir_;
	}

	::memset(&fsInfo_, '\0', sizeof(fsInfo_));
	devicePartition_ = findPartition();
}


const struct statvfs &
DiskInfo::getFsStats()
{
	(void) loadFsStats();

	return fsInfo_;
}


long
DiskInfo::getFsXferBlockSize()
{
	long retVal;

	if (fsInfo_.f_bsize == 0)
	{
		int stat = loadFsStats();

		if (stat == -1)
		{
			// Just pick something reasonable
			//
			return 1024 * 8;
		}
	}

	retVal = (long) fsInfo_.f_bsize;

	return retVal;
}


int
DiskInfo::loadFsStats()
{
	int retVal = 0;

	retVal = ::statvfs(trueDir_.get(), &fsInfo_);

	if (retVal == -1)
	{
		retVal = errno;
	}

	return retVal;
}


char_ptr
DiskInfo::findPartition()
{
	char_ptr retVal = ptr_strdup("unknown");

	FILE *f = ::fopen("/proc/self/mounts", "r");

	if (f != (FILE *) 0)
	{
		int num;
		int maxMatch;
		char partition[128];
		char mount[128];

		while ((num = fscanf(f, "%s %s", partition, mount)) != EOF)
		{
			if (::strstr(trueDir_.get(), mount) == trueDir_.get())
			{
				int l = strlen(mount);

				if (l > maxMatch)
				{
					retVal = ptr_strdup(partition);
					maxMatch = l;
				}
			}

			while ((num = fgetc(f)) != EOF)
			{
				if (num == '\n')
				{
					break;
				}
			}
		}

		(void) ::fclose(f);
	}

	return retVal;
}


bool
DiskInfo::getDiskStats(DiskInfo::DiskStats &ds)
{
	bool retVal = false;
	FILE *f = ::fopen("/proc/diskstats", "r");

	if ((f != (FILE *) 0) && (devicePartition_.get() != (char *) 0))
	{
		char *d = strdup_wnew(devicePartition_.get());
#ifdef __linux
		char *d2 = ::basename(d);
#else
		char *d2 = ::rindex(d, '/');
		if (d2 != (char *) 0)
		{
			d2++;
		}
#endif
		int num;
		int maj, min;
		DiskInfo::DiskStats t;
		char dev[128];

		while (strlen(d2) > 0 && isdigit(d2[strlen(d2) - 1]))
		{
			d2[strlen(d2) - 1] = '\0';
		}

		while ((num = fscanf(f, "%d %d %s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", 
			&maj, &min, dev, &t.rd_compl, &t.rd_merg, &t.rd_sectors, &t.rd_ms,
			&t.wr_compl, &t.wr_merg, &t.wr_sectors, &t.wr_ms, &t.io_curr, &t.io_ms,
			&t.io_weight)) != EOF)
		{

			if (::strcmp(dev, d2) == 0)
			{
				(void) ::memcpy(&ds, &t, sizeof(ds));
				retVal = true;
				break;
			}

			while ((num = fgetc(f)) != EOF)
			{
				if (num == '\n')
				{
					break;
				}
			}
		}
		(void) ::fclose(f);
		delete [] d;
	}

	return retVal;
}


bool
DiskInfo::getPartitionStats(DiskInfo::PartitionStats &ps)
{
	bool retVal = false;
	FILE *f = ::fopen("/proc/diskstats", "r");

	if ((f != (FILE *) 0) && (devicePartition_.get() != (char *) 0))
	{
		char *d = strdup_wnew(devicePartition_.get());
#ifdef __linux
		char *d2 = ::basename(d);
#else
		char *d2 = ::rindex(d, '/');
		if (d2 != (char *) 0)
		{
			d2++;
		}
#endif
		int num;
		int maj, min;
		DiskInfo::PartitionStats t;
		char dev[128];

		while ((num = fscanf(f, "%d %d %s %lu %lu %lu %lu",
			&maj, &min, dev, &t.rd_compl, &t.rd_sectors,
			&t.wr_compl, &t.wr_sectors)) != EOF)
		{

			if (::strcmp(dev, d2) == 0)
			{
				(void) ::memcpy(&ps, &t, sizeof(ps));
				retVal = true;
				break;
			}

			while ((num = fgetc(f)) != EOF)
			{
				if (num == '\n')
				{
					break;
				}
			}
		}
		(void) ::fclose(f);
		delete [] d;
	}

	return retVal;
}

void
DiskInfo::getInfo(Config &cfg)
{
	DiskStats ds;
	const char *s = trueDir_.get();

	if (s != (const char *) 0)
	{
		cfg.addString("BASE_DIR", s, true);
	}

	if (loadFsStats() == 0)
	{
		addLong(cfg, "INODE_FREE", fsInfo_.f_favail);
		addLong(cfg, "INODE_TOTAL", fsInfo_.f_files);
		addLong(cfg, "BLOCK_FREE", fsInfo_.f_bavail);
		addLong(cfg, "BLOCK_TOTAL", fsInfo_.f_blocks);
		addLong(cfg, "BLOCK_SIZE", fsInfo_.f_frsize);
	}

	if (getDiskStats(ds))
	{
		addLong(cfg, "READ_SECTORS", ds.rd_sectors);
		addLong(cfg, "WRITE_SECTORS", ds.wr_sectors);
		addLong(cfg, "IO_WEIGHT", ds.io_weight);
	}
}

//-----------------------------

NetInfo::NetInfo(const char *nic)
{
	if (nic != (const char *) 0)
	{
		nic_ = ptr_strdup(nic);
	}
	init();
}

NetInfo::NetInfo(const char_ptr nic) : nic_(nic)
{
	init();
}


NetInfo::~NetInfo()
{
}


void
NetInfo::init() throw ()
{

	int st = netwrap::getIpAddr(nic_.get(), &ipAddr_);

	if (st != 0)
	{
		APPLOG_WARN("Failed to find the ipaddr for %s; code=%d", nic_.get(),
			st);
	}
	else
	{
#if LINUX_VERSION_CODE >= 132608
		struct ifaddrs *ifaddr;
		int st = getifaddrs(&ifaddr);

		if (st == 0)
		{
			struct ifaddrs *cur = ifaddr;
			struct sockaddr_in *in;

			while (cur != (struct ifaddrs *) 0)
			{
				in = (struct sockaddr_in *) cur->ifa_addr;

				if (in->sin_addr.s_addr == ipAddr_)
				{
					char buf[64];

					inet_ntop(AF_INET, &(in->sin_addr), buf, sizeof(buf));
					ipStr_ = ptr_strdup(buf);
					if (cur->ifa_name != (char *) 0)
					{
						if_ = ptr_strdup(cur->ifa_name);
					}

					break;
				}
				cur = cur->ifa_next;
			}		
			freeifaddrs(ifaddr);
		}
		else
		{
			APPLOG_WARN("Failed in getifaddrs(): errno=%d", errno);
		}
#endif
	}
}


bool
NetInfo::getIfStats(NetInfo::IfStats &is)
{
	bool retVal = false;
	FILE *f = fopen("/proc/net/dev", "r");

	if (f != (FILE *) 0)
	{
		char buf[1024];
		NetInfo::IfStats t;
		int num;

		// Skip first 2 header lines:
		//
		while (fgetc(f) != '\n') ;
		while (fgetc(f) != '\n') ;

		while ((num = fscanf(f, "%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
			buf, &t.recv_bytes, &t.recv_pkt, &t.recv_err, &t.recv_drop,
			&t.recv_fifo, &t.recv_frame, &t.recv_compr, &t.recv_mcast,
			&t.tran_bytes, &t.tran_pkt, &t.tran_err, &t.tran_drop, &t.tran_fifo,
			&t.tran_colls, &t.tran_carr, &t.tran_compr)) != EOF)
		{

			while ((num = fgetc(f)) != EOF)
			{
				if (num == '\n')
				{
					break;
				}
			}
		}

		(void) ::fclose(f);
	}

	return retVal;
}


void
NetInfo::getInfo(Config &cfg)
{
	IfStats is;
	const char *s = ipStr_.get();

	if (s != (char *) 0)
	{
		cfg.addString("IP_ADDR", s, true);
	}

	if (getIfStats(is))
	{
		addLong(cfg, "NET_RD", is.recv_pkt);
		addLong(cfg, "NET_WR", is.tran_pkt);
	}
}


//-----------------------------


ProcessInfo::ProcessInfo() throw ()
{
    (void) ::gettimeofday(&startT_, 0);
	pid_ = ::getpid();
}


ProcessInfo::~ProcessInfo() throw ()
{
}


int
ProcessInfo::setMaxFD(int newMax) throw ()
{
	int retVal = 0;
	struct rlimit lim;

	lim.rlim_cur = newMax;

	int ok = ::setrlimit(RLIMIT_NOFILE, &lim);

	if (ok == -1)
	{
		retVal = errno;
	}
	
	return retVal;
}


int
ProcessInfo::getMaxFD() throw ()
{
	int retVal = ::getdtablesize();

	return retVal;
}


int
ProcessInfo::getCurrentFDUsage() throw ()
{
	int retVal = 0;
	DIR *d = ::opendir("/proc/self/fd");

	if (d == (DIR *) 0)
	{
		retVal = -1;
	}
	else
	{
		while (::readdir(d) != (struct dirent *) 0) {
			retVal++;
		}
		(void) ::closedir(d);

		// This is for ".", "..", and the opendir we just did.
		retVal -= 3;
	}

	// This appears to be slower than the above method, so we'll only
	// do it as a last resort.
	//
	if (retVal <= -1)
	{
		int maxFd = ::getdtablesize();
		struct pollfd stuff[maxFd];

		for (int i = 0; i < maxFd; i++)
		{
			stuff[i].fd = i;
			stuff[i].events = POLLIN | POLLOUT;
			stuff[i].revents = 0;
		}

		int s = ::poll(stuff, maxFd, 0);

		if (s <= 0)
		{
			retVal = -1;
		}
		else
		{
			retVal = 0;
			for (int i = 0; i < maxFd; i++)
			{
				if ((stuff[i].revents & POLLNVAL) == 0)
				{
					retVal++;
				}
			}
		}
	}

	return retVal;
}



char_ptr
ProcessInfo::getCwd() const
{
	return readSymlink("/proc/self/cwd");
}


char_ptr
ProcessInfo::getExe() const
{
	return readSymlink("/proc/self/exe");
}


char_ptr
ProcessInfo::getCmdline() const
{
	char_ptr retVal;
	FILE *f = ::fopen("/proc/self/cmdline", "r");

	if (f != (FILE *) 0)
	{
		char buf[4096];

		size_t numRd = ::fread(buf, sizeof(char), sizeof(buf), f);

		for (size_t i = 0; i < numRd; i++)
		{
			if (buf[i] == '\0')
			{
				buf[i] = ' ';
			}
		}
		buf[numRd] = '\0';
		retVal = ptr_strdup(buf);

		(void) ::fclose(f);
	}

	return retVal;
}



long
ProcessInfo::getStartTime() const throw ()
{
	long retVal = -1;
	struct stat buf;
	int st = ::stat("/proc/self", &buf);

	if (st == 0)
	{
		retVal = buf.st_ctime;
	}

	return retVal;
}


void
ProcessInfo::getInfo(Config &cfg)
{
	char_ptr p;
	char *s;
	struct rusage r;

	p = getCmdline();
	if ((s = p.get()) != (char *) 0)
	{
		cfg.addString("CMD_LINE", s, true);
	}

	p = getCwd();
	if ((s = p.get()) != (char *) 0)
	{
		cfg.addString("CWD", s, true);
	}

	p = getExe();
	if ((s = p.get()) != (char *) 0)
	{
		cfg.addString("EXE", s, true);
	}
	addLong(cfg, "PID", getPid());
	addLong(cfg, "CURR_FD", getCurrentFDUsage());


	if (getUsage(&r) == 0)
	{
		addLong(cfg, "PROC_RSS", r.ru_maxrss);
		addLong(cfg, "PROC_HEAP", r.ru_idrss); //@@TODO: is this correct interp?
		addLong(cfg, "PROC_STACK", r.ru_isrss);
		addLong(cfg, "PROC_IO_OUT", r.ru_oublock);
		addLong(cfg, "PROC_IO_IN", r.ru_inblock);
		addLong(cfg, "PROC_PAGE_FAULTS", r.ru_majflt);
		addLong(cfg, "PROC_SWAP", r.ru_nswap);
	}
}


int
ProcessInfo::getUsage(struct rusage *ru) throw ()
{
	int retVal = ::getrusage(RUSAGE_SELF, ru);

	return retVal;
}


//--------------------


SystemInfo::SystemInfo()
{
	init();
}


SystemInfo::~SystemInfo()
{
}


void
SystemInfo::init() throw ()
{
	(void) ::uname(&uname_);
}


long
SystemInfo::getMaxFD() const throw ()
{
	// Can this be tuned on-the-fly?  Don't know, so we'll re-query each
	// time instead of caching.
	//
	long retVal = ::sysconf(_SC_OPEN_MAX);

	return retVal;
}


long
SystemInfo::getUptime() const
{
	long retVal = -1;
	FILE *f = ::fopen("/proc/uptime", "r");

	if (f != (FILE *) 0)
	{
		// Not quite sure what the format is, but this is good enough.
		//
		::fscanf(f, "%ld", &retVal);

		(void) ::fclose(f);
	}

	return retVal;
}


bool
SystemInfo::getLoadAvg(float &avg1, float &avg2, float &avg3)
{
	bool retVal = false;
	FILE *f = ::fopen("/proc/loadavg", "r");

	if (f != (FILE *) 0)
	{
		int NUM_ARGS = 6;
		int numRunning;
		int numProcs;
		int lastPid;

		int numAssigned = fscanf(f, "%f %f %f %d/%d %d", &avg1, &avg2, &avg3, &numRunning, &numProcs, &lastPid);

		if (numAssigned == NUM_ARGS)
		{
			retVal = true;
		}

		(void) ::fclose(f);
	}

	return retVal;
}


void
SystemInfo::getInfo(Config &cfg)
{
	cfg.addString("HOST", getHostname(), true);
	cfg.addString("OS_VERSION", uname_.release, true);
	addLong(cfg, "MAX_FD", getMaxFD());
	addLong(cfg, "SYS_UPTIME", getUptime());

	float avg1, avg2, avg3;

	if (getLoadAvg(avg1, avg2, avg3))
	{
		addDouble(cfg, "SYS_LOAD1", avg1);
		addDouble(cfg, "SYS_LOAD2", avg2);
		addDouble(cfg, "SYS_LOAD3", avg3);
	}

#ifdef __linux
    struct sysinfo sysinfo_;

	if (::sysinfo(&sysinfo_) == 0)
	{
		addLong(cfg, "NUM_PROCS", sysinfo_.procs);
		addLong(cfg, "RAM_TOTAL", sysinfo_.totalram);
		addLong(cfg, "RAM_FREE", sysinfo_.freeram);
		addLong(cfg, "SWAP_TOTAL", sysinfo_.totalswap);
		addLong(cfg, "SWAP_FREE", sysinfo_.freeswap);
	}
#endif
}

