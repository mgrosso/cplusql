
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#include "AppLog.h"
#include "AppConfig.h"
#include "clusterioC.h"
#include "DateUtils.h"
#include "FileHandler.h"
#include "keys.h"
#include "netwrap.h"
#include "NVPairUtil.h"
#include "RWSExceptions.h"
#include "ReadWriteController.h"
#include "SharedServiceMain.h"



AppConfig *appConfig = (AppConfig *) 0;
clusterio::open_info global_fd;


void setupFileId(clusterio::file_handle &fh, const char *fname)
{
	fh.id = ::atoll(fname);
}


void setupServants(clusterio::open_info &fd)
{
	char buf[128];

	try {
		for (int i = 1; true; i++)
		{
			(void) ::sprintf(buf, "GS_URL%d", i);

			const char * rws_url = appConfig->getString(buf);

			if (rws_url == (const char *) 0)
			{
				break;
			}
			else
			{
				CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve( rws_url );
				if (obj == 0)
				{
					(void) ::fprintf(stderr, "Failed to resolve %s=%s\n", buf, rws_url);
					::exit(1);
				}
				else
				{
					fd.servants.length(i);
//@	@TODO				fd.servants[i - 1].ior = obj.get().ior().
					fd.servants[i - 1].serviceName = RWS_NAME;
					fd.servants[i - 1].url = rws_url;
				}
			}
		}
	} catch (ConfigNoSuchKey &nsk) {
		; // Ignore
	}
}


void setupWriterId(clusterio::writer_id &writer, const char *wid)
{
	if (wid == (const char *) 0)
	{
		writer.length(0);
	}
	else
	{
		size_t len = ::min((size_t) 8, strlen(wid));
		writer.length(len);
		for (size_t i = 0; i < len; i++)
			writer[i] = wid[i];
	}
}


void
opCp(clusterio::ReadWriteService_ptr destRws, const char *destFile,
	const char *srcRws, const char *srcFile)
{
	Service::ServiceRef srcRef;
	clusterio::file_handle destFd;
	clusterio::file_handle srcFd;

	srcRef.url = srcRws;

	setupFileId(destFd, destFile);
	setupFileId(srcFd, srcFile);
	
	destRws->namespace_cp(destFd, srcRef, srcFd);
}


void
opDump(clusterio::ReadWriteService_ptr rws, const char *what)
{
	CORBA::String_var os;

	rws->dumpState(what, clusterio::ReadWriteService::flat,
		clusterio::ReadWriteService::full, os);

	printf("state=%s\n", os.in());
}


void
opFail(clusterio::ReadWriteService_ptr rws, const char *args)
{
	char *s = ::strdup(args);
	Service::NVPairList *nvp = NVPairUtil::convert(s, ',');

	free(s);

	if (nvp != (Service::NVPairList *) 0)
	{
		APPLOG_DBG("calling forcedFailureConfiguration %s", args);
		rws->forcedFailureConfiguration(*nvp);

		delete nvp;

		APPLOG_DBG("returing from forcedFailureConfiguration");
	}
}


void
opPing(clusterio::ReadWriteService_ptr rws)
{
	APPLOG_DBG("calling ping");
	rws->ping();
	APPLOG_DBG("returned from ping");
}


void
opPullStatus(clusterio::ReadWriteService_ptr rws)
{
	Service::ServiceStatus *ss = rws->pullStatus();

	(void) ::printf("statusReceivedTime=%ld\n", ss->statusReceivedTime);
	(void) ::printf("pid=%d\n", ss->pid);
	(void) ::printf("isPendingShutdown=%d\n", (int) ss->isPendingShutdown);
	(void) ::printf("live=%d\n", ss->live);

	int sz = ss->details.length();
	for (int i = 0; i < sz; i++) {
		Service::NVPair &p = ss->details[i];
		const char *name = p.name;
		const char *value = p.value;

		(void) ::printf("\t%s=%s\n", name, value);
	}
}


void
opRestart(clusterio::ReadWriteService_ptr rws)
{
	rws->restart();
}


void
opReconfigure(clusterio::ReadWriteService_ptr rws, char *data)
{
	Service::NVPairList *l = NVPairUtil::convert(data, '&');

	rws->reconfigure(*l);

	if (l != (Service::NVPairList *) 0)
	{
		delete l;
	}
}


size_t
opRead(clusterio::ReadWriteService_ptr rws, const char *wid, const char *fname, size_t off, size_t len, char *buf, FILE *fp)
{
	clusterio::open_info fd;
	clusterio::writer_id writer;

	setupFileId(fd.handle, fname);
	setupWriterId(writer, wid);

	fd.handle.flags = clusterio::o_rdonly;

	clusterio::file_data_t_var v;
	clusterio::file_data_t_out wrapper(v);

	(void) ::printf("Going to read off=%llu, len=%llu\n", off, len);

	rws->read(fd, off, len, wrapper);

	clusterio::file_data_t *t = wrapper.ptr();

	size_t retVal = t->length();

	::memset(buf, '\0', len);
	CORBA::Octet *ob = t->get_buffer();
	char *s = reinterpret_cast<char *>(ob);

	::memcpy(buf, s, retVal);

	if (fp != (FILE *) 0)
	{
		(void) ::fwrite(buf, sizeof(char), retVal, fp);
		(void) ::fflush(fp);
	}

	return retVal;
}


void
opReadToFile(clusterio::ReadWriteService_ptr rws, const char *wid, const char *fname, const char *locFile, size_t bufSize)
{
	const char *needle = "file ";
	char *at = ::strstr(locFile, needle) + ::strlen(needle);
	FILE *f = ::fopen(at, "w");

	if (f == (FILE *) 0)
	{
		(void) ::fprintf(stderr, "error: couldn't open %s\n", at);
	}
	else
	{
		char buf[bufSize + 1];
		size_t off = 0;

		::memset(buf, '\0', bufSize + 1);

		try {
			while (true)
			{
				size_t numRd = opRead(rws, wid, fname, off, bufSize, buf, f);
				off += numRd;
				if (numRd == 0)
				{
					break;
				}
			}
		} catch (clusterio::ClusterIOException &ce) {
			(void) ::fprintf(stderr, "Caught an ClusterIOException\n");
		} catch (Service::ServiceException &se) {
			(void) ::fprintf(stderr, "Caught an ServiceException\n");
		} catch (CORBA::SystemException &cse) {
			(void) ::fprintf(stderr, "Caught an CORBA::SystemException\n");
		} catch (...) {
			(void) ::fprintf(stderr, "Caught an unknown\n");
		}
		(void) ::fclose(f);
	}
}


void
opRm(clusterio::ReadWriteService_ptr rws, const char *fname, const char *debug)
{
	clusterio::file_handle handle;

	setupFileId(handle, fname);
	handle.flags = 0;
	handle.debug_info = debug;
	handle.append_token = 0;

	rws->namespace_rm(handle);
}


void
opShutdown(clusterio::ReadWriteService_ptr rws)
{
	rws->gracefulShutdown();
}


void
opStat(clusterio::ReadWriteService_ptr rws, const char *fname)
{
    clusterio::file_handle handle;

	setupFileId(handle, fname);
    handle.flags = 0;
    handle.append_token = 0;

	clusterio::file_handle_seq seq;
	seq.length(1);
	seq[0] = handle;

	clusterio::file_handle_status_seq *ret = rws->stat(seq, true);

	if (ret != (clusterio::file_handle_status_seq *) 0)
	{
		int sz = ret->length();

		(void) ::printf("sz=%d\n", sz);
		for (int i = 0; i < sz; i++)
		{
			clusterio::file_handle_status fhs = (*ret)[i];

			(void) ::printf("%d: %d\n", i, fhs.status);

			int sz2 = fhs.data_ranges.length();
			for (int j = 0; j < sz2; j++)
			{
				clusterio::data_range dr = fhs.data_ranges[j];

				(void) ::printf("\t%d: writerId=%s, seqNo=%ld logical offset=%ld len=%ld\n", j, dr.writer.get_buffer(), (long) dr.writers_sequence, (long) dr.offset, (long) dr.length);
			}
		}
	}
}


void
opStatRead(clusterio::ReadWriteService_ptr rws, const char *wid, const char *fname, FILE *fp)
{
	clusterio::file_handle handle;

	setupFileId(handle, fname);
	handle.flags = 0;
	handle.append_token = 0;

	clusterio::file_handle_seq seq;
	seq.length(1);
	seq[0] = handle;

	clusterio::file_handle_status_seq *ret = rws->stat(seq, true);

	if (ret != (clusterio::file_handle_status_seq *) 0)
	{
		int sz = ret->length();

		for (int i = 0; i < sz; i++)
		{
			clusterio::file_handle_status fhs = (*ret)[i];

			int sz2 = fhs.data_ranges.length();

			for (int j = 0; j < sz2; j++)
			{
				clusterio::data_range dr = fhs.data_ranges[j];
				char buf[dr.length + 1];

//				(void) ::printf("\t%d: writerId=%s, seqNo=%ld logical offset=%ld len=%ld\n", j, dr.writer.get_buffer(), (long) dr.writers_sequence, (long) dr.offset, (long) dr.length);
				opRead(rws, wid, fname, dr.offset, dr.length, buf, fp);
			}
		}
	}
}


void
opUndoAppend(clusterio::ReadWriteService_ptr rws, char *wid, const char *fname)
{
	clusterio::open_info fd;
	clusterio::writer_id writer;

	fd = global_fd;
	setupFileId(fd.handle, fname);
//	setupServants(fd);
	setupWriterId(writer, wid);

	(void) ::printf("undoAppend(wid=%s) fname=%s\n", wid, fname);
	CORBA::ULong nextSeqNo = 0;
	CORBA::ULong_out out = nextSeqNo;
	clusterio::length_t bytesUndone = rws->undo_appends(fd, writer, out);

	(void) ::printf("undoAppend(wid=%s) Bytes undone=%lld nextSeq=%ld\n", wid, bytesUndone, (long) nextSeqNo);
}


void
opAppend(clusterio::ReadWriteService_ptr rws, const char *txt, size_t len, char *wid, const char *fname, long seqNum, const char *debug)
{
	clusterio::open_info fd;
	clusterio::writer_id writer;

	fd = global_fd;
	setupFileId(fd.handle, fname);
	setupWriterId(writer, wid);

	fd.handle.flags = clusterio::o_wronly;
	fd.handle.debug_info = debug;

//	printf("opAppend %ld %ld\n", len, seqNum);
//	setupServants(fd);

	clusterio::block blk;

	if (len == 0)
	{
		len = ::strlen(txt);
	}

	blk.data.length(len);
	for (size_t i = 0; i < len; i++)
		blk.data[i] = txt[i];

	blk.check_type = clusterio::none;
	blk.checksums.length(4);
	for (int i = 0; i < 4; i++)
		blk.checksums[i] = 0;

	clusterio::blockseq blocks;

	blocks.length(1);
	blocks[0] = blk;
	
	rws->append(fd, blocks, writer, seqNum);
}



void
opAppendFile(clusterio::ReadWriteService_ptr rws, const char *locFile, char *wid, const char *fname, size_t blockSize, size_t seqNo, const char *debug)
{
	const char *needle = "file ";
	char *at = ::strstr(locFile, needle) + ::strlen(needle);
	FILE *f = ::fopen(at, "r");

	if (f == (FILE *) 0)
	{
		(void) ::fprintf(stderr, "error: couldn't open %s\n", at);
	}
	else
	{
		char buf[blockSize + 1];
		size_t numRd;

		::memset(buf, '\0', blockSize + 1);
		while ((numRd = ::fread(buf, sizeof(char), blockSize, f)) > 0)
		{
			opAppend(rws, buf, numRd, wid, fname, seqNo++, debug);
			::memset(buf, '\0', blockSize + 1);
		}
		(void) ::fclose(f);
	}
}


int main(int argc, char *argv[])
{
	enum Op {
		None = 0,
		Append,
		Cp,
		Dump,
		Fail,
		Ping,
		PullStatus,
		Read,
		Reconfigure,
		Restart,
		Rm,
		Shutdown,
		Stat,
		StatRead,
		UndoAppend
	};
	struct NameOp {
		const char *nm;
		Op op;
	};

	struct NameOp ops[] = {
		{ "none", None},
		{ "append", Append},
		{ "cp", Cp},
		{ "dump", Dump},
		{ "fail", Fail},
		{ "ping", Ping},
		{ "pull", PullStatus},
		{ "reconfigure", Reconfigure},
		{ "read", Read},
		{ "reconfigure", Reconfigure},
		{ "restart", Restart},
		{ "rm", Rm},
		{ "shutdown", Shutdown},
		{ "stat", Stat},
		{ "statread", StatRead},
		{ "undo", UndoAppend},
		{0, None}
	};


	char_ptr args;
	char_ptr writerId;
	char_ptr fname;
	char_ptr text;
	char_ptr debug;
	long seqNo;
	int blockSize = 1024;

	try {
		appConfig = AppConfig::instance();

		appConfig->init(argc, argv);
		appConfig->consumeArgs(&argc, argv);
		appConfig->addString( "APPLOG_MINLOG", "0", false );
		AppLog::instance()->reconfigure( *appConfig );

		int arg;
		Op op;


		while ((arg = ::getopt(argc, argv, "a:b:d:f:o:s:t:w:")) != -1) {
			switch (arg) {
			case 'a':
				args = ptr_strdup(optarg);
				break;

			case 'b':
				blockSize = atoi(optarg);
				break;

			case 'd':
				debug = ptr_strdup(optarg);
				break;

			case 'f':
				fname = ptr_strdup(optarg);
				break;

			case 'o':
				op = None;
				for (int i = 0; ops[i].nm != (const char *) 0; i++)
				{
					if (::strcasecmp(optarg, ops[i].nm) == 0)
					{
						op = ops[i].op;
						break;
					}
				}
				if (op == None)
				{
					(void) ::fprintf(stderr, "Unknown op %s\n", optarg);
					::exit(1);
				}
				break;

			case 's':
				seqNo = ::atol(optarg);
				break;

			case 't':
				text = ptr_strdup(optarg);
				break;

			case 'w':
				writerId = ptr_strdup(optarg);
				break;

			default:
				break;
			}
		}

		// First initialize the ORB, that will remove some arguments...
		SharedServiceMain::init( argc, argv, __FILE__, false );

		const char * rws_url = appConfig->getString( "GS_URL" );

		if (rws_url == (const char *) 0)
		{
			(void) ::fprintf(stderr, "$GS_URL must be specified\n");
			::exit(1);
		}

		APPLOG_DBG("find GS_URL=%s", rws_url);

		CORBA::Object_ptr obj = SharedServiceMain::instance()->resolve( rws_url );

		if (obj == NULL)
		{
			PANICV("Could not create object from string");
			::exit(1); // not reached
		}
		clusterio::ReadWriteService_ptr rws = clusterio::ReadWriteService::_narrow( obj );

		setupServants(global_fd);
		switch (op) {
		case Append:
			struct timeval startT;

			DateUtils::getTime(startT);

			if (::strstr(text.get(), "file ") == text.get())
			{
				opAppendFile(rws, text.get(), writerId.get(), fname.get(), blockSize, seqNo, debug.get());
			}
			else
			{
				for (int i = 0; i < 1; i++)
				{
//					size_t len = 1024 * 1024;
//					char *buf = new char[len];
//
//					::memset(buf, '1', len);
//					buf[len - 1] = '\0';
//					(void) ::printf("before len=%ld", strlen(buf));

					opAppend(rws, text.get(), 0, writerId.get(), fname.get(), seqNo + i, debug.get());
				}

				double d = DateUtils::diffTime(startT);
				(void) ::printf("total time=%.2f\n", d);
			}

			break;

		case Cp:
			opCp(rws, fname.get(), text.get(), args.get());
			break;

		case Dump:
			opDump(rws, text.get());
			break;

		case Fail:
			opFail(rws, args.get());
			break;

		case Ping:
			opPing(rws);
			break;

		case PullStatus:
			opPullStatus(rws);
			break;

		case Read:
			if ((text.get() != 0) && (::strstr(text.get(), "file ") == text.get()))
			{
				opReadToFile(rws, writerId.get(), fname.get(), text.get(), blockSize);
			}
			else
			{
				size_t off;
				size_t len;

				(void) ::printf("args=%s\n", args.get());
				::sscanf(args.get(), "%lu %lu", &off, &len);

				(void) ::printf("off=%ld, len=%ld\n", off, len);

				char buf[len];
				size_t numRd = opRead(rws, writerId.get(), fname.get(), off, len, buf, stdout);

				(void) ::printf("bytes read=%lu\n", numRd);
			}
			break;

		case Reconfigure:
			opReconfigure(rws, text.get());
			break;

		case Restart:
			opRestart(rws);
			break;

		case Rm:
			opRm(rws, fname.get(), debug.get());
			break;

		case Shutdown:
			opShutdown(rws);
			break;

		case Stat:
			opStat(rws, fname.get());
			break;

		case StatRead:
			{
				FILE *fp = stdout;
				char *locFile = text.get();
				if ((locFile != 0) && (::strstr(locFile, "file ") == locFile)) {
					const char *needle = "file ";
					char *at = ::strstr(locFile, needle) + ::strlen(needle);

					fp = ::fopen(at, "w");
				}
				opStatRead(rws, writerId.get(), fname.get(), fp);
			}
			break;

		case UndoAppend:
			opUndoAppend(rws, writerId.get(), fname.get());
			break;

		default:
			(void) ::fprintf(stderr, "Unhandled op\n");
			break;
		}
	} catch (clusterio::ClusterIOException &cie) {
		for (size_t i = 0; i < cie.exceptions.length(); i++)
		{
			(void) ::fprintf(stderr, "caught clusterioException\n");
			(void) ::fprintf(stderr, "\turl=%s\n", cie.exceptions[i].serviceRef.url.in());
			(void) ::fprintf(stderr, "\ttype=%d\n", cie.exceptions[i].type);
			(void) ::fprintf(stderr, "\terrno=%d\n", cie.exceptions[i].errno_short);
			(void) ::fprintf(stderr, "\twhat=%s\n", cie.exceptions[i].what_string.in());
		}
	} catch (Service::PendingShutdownException &pse) {
		(void) ::fprintf(stderr, "caught pending shutdown");
	} catch (Service::ServiceException &se) {
		(void) ::fprintf(stderr, "caught serviceException %s\n", se.what_string.in());
	} catch (CORBA::SystemException &se) {
		(void) ::fprintf(stderr, "%s caught CORBA::SystemException: %s\n", writerId.get(), se._info().rep());
		se._tao_print_system_exception(stderr);
	} catch (exception &e) {
		(void) ::fprintf(stderr, "caught exception in main: %s\n", e.what());
	} catch (...) {
		(void) ::fprintf(stderr, "Caught something\n");
	}

	SharedServiceMain::shutdown();
}
