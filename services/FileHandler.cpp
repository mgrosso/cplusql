
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>

#include "AppLog.h"
#include "FileHandler.h"
#include "ReadWriteController.h"
#include "RWSRunnable.h"
#include "ThreadRunnable.h"


using namespace rws;


FileHandler::FileHandler(ReadWriteController &rwc, clusterio::length_t id, const char *fname, const char *aSfx, const char *fSfx)
	: rwc_(rwc), privateId_(id), fname_((char *) 0), auditor_(*this, fname, aSfx), fio_(*this, fname, fSfx), leadingAdj_(0), trailingAdj_(0), needFixup_(false)
{
	appendToken_ = 0;
	fname_ = ptr_strdup(fname);
}


FileHandler::~FileHandler()
{
	for (WriterRecords::iterator itor = widInfo_.begin(); itor != widInfo_.end(); ++itor)
	{
		WriterInfo *wi = itor->second;

		if (wi != (WriterInfo *) 0)
		{
			if (wi->recs != (WriterAdj *) 0)
			{
				// Don't delete the records inside the list, since those are
				// really owned by goodRecods_.
				//
				delete wi->recs;
				wi->recs = (WriterAdj *) 0;
			}
			delete wi;
		}
	}
	widInfo_.clear();
	goodRecords_.clear();
}


void
FileHandler::init()
{
	auditor_.init();
	fio_.init();

	APPLOG_DBG("auditor.file=%s", auditor_.getFilename());
	APPLOG_DBG("fio.file=%s", fio_.getFilename());

}


int
FileHandler::getCurrentUserCount() const
{
	int retVal = 0;

	semCtr_.getvalue(&retVal);
	retVal -= 1;

	return retVal;
}


void
FileHandler::loadAuditLogForAppend()
{
	MutexGuard guard(mtx_);

	if (widInfo_.size() == 0)
	{
		AuditLog::ReadContext ctxt;

		APPLOG_DBG("loadAuditLogForAppend start");
		mode_ = Append;

		try {
			while (auditor_.readNext(ctxt))
			{
				AuditLog::Entry &e = ctxt.getEntry();

				APPLOG_DBG("loadAuditForAppend: wid=%s, seqNo=%ld", e.writerId, e.seqNo);
				adjRecords(e);
			}
			auditor_.closeReader();
		} catch (rws::ReadException &e) {
			; //@@TODO
		} catch (rws::Exception &e) {
			; // Ignore Open/Close
		}
		APPLOG_DBG("loadAuditLogForAppend end");
	}
}


void
FileHandler::loadAuditLogForRead()
{
	MutexGuard guard(mtx_);

	if (widInfo_.size() == 0)
//	if (mode_ != Read)
	{
		AuditLog::ReadContext ctxt;

		APPLOG_DBG("loadAuditLogForRead start");

		mode_ = Read;

		try {
			while (auditor_.readNext(ctxt))
			{
				AuditLog::Entry &e = ctxt.getEntry();

				adjRecords(e);
			}
		} catch (rws::ReadException &e) {
			; //@@TODO
		} catch (rws::Exception &e) {
			; // Ignore Open/Close
		}

		APPLOG_DBG("loadAuditLogForRead end");
	}
}


list<AuditLog::Entry> *
FileHandler::getValidEntries()
{
	list<AuditLog::Entry> *retVal = new list<AuditLog::Entry>();
	Counter ctr(semCtr_);

	loadAuditLogForRead();
	fixupGoodList();

	MutexGuard guard(mtx_);

	for (DLList::Node *n = goodRecords_.head; n != (DLList::Node *) 0; n = n->next)
	{
		RecordAdj *ra = reinterpret_cast<RecordAdj *>(n);
		AuditLog::Entry e = ra->entry;

		e.offset = ra->logOffset;

		retVal->push_back(e);
	}

	if (retVal != 0)
		APPLOG_INFO("getValidEntries returning %ld", (long) retVal->size());

	return retVal;
}


void
FileHandler::verifyChecksum(const clusterio::block &blk, int blockNo)
throw (BadDataException)
{
	if (blk.check_type == clusterio::none)
		;
	else if (blk.check_type == clusterio::crc48)
	{
		CRC48HashAlg *inst = CRC48HashAlg::getInstance();
		const CORBA::Octet *p = blk.data.get_buffer();
		size_t len = blk.data.length();

		unsigned long long ck = inst->getCRC48(const_cast<CORBA::Octet *>(p), len);

		if (ck != blk.checksums[0])
		{
			throw BadDataException(NICE_LOC, Resource(blockNo), Args(len, blk.checksums[0], ck), Status("Corrupt CRC48"));
		}
	}
	else
	{
		throw BadDataException(NICE_LOC, Resource(blockNo), Status("Unknown crc type"));
	}
}


void
FileHandler::append(const clusterio::blockseq &blocks, const char *wid, size_t seqNum)
throw (OpenException, WriteException, BadDataException)
{
	Counter ctr(semCtr_);
	bool seqNumOk;

	FAIL_FH_IF("fhAppendSleep", ::sleep(FAIL_FH_INT("fhAppendSleep")))

	loadAuditLogForAppend();

	APPLOG_INFO("Doing append for %s", wid);

	{
		MutexGuard guard(mtx_);

		seqNumOk = checkWriterSeqNo(wid, seqNum);
	}

	if (seqNumOk)
	{
		int sz = blocks.length();

		for (int i = 0; i < sz; i++)
		{
			const clusterio::block &blk = blocks[i];
			const CORBA::Octet *p = blk.data.get_buffer();

			verifyChecksum(blk, 0);
			APPLOG_INFO("block %d, checksum ok. len=%ld", i, blk.data.length());

			append(wid, seqNum, reinterpret_cast<const char *>(p), blk.data.length(), blk.check_type, &blk.checksums);
		}
	}
	else
	{
		APPLOG_WARN("Bad seqNum #%ld, ignoring append for wid=%s", seqNum, wid);
	}
}


void
FileHandler::append(const char *writerId, unsigned long seqNo, const char *data, ssize_t len, clusterio::e_checksum_type type, const clusterio::checksum_t *sum)
	throw (OpenException, WriteException)
{
	Counter ctr(semCtr_);
	FileIO::AppendContext ctxt;
	struct timeval nowT;

	loadAuditLogForAppend();

	if (len < 0)
	{
		len = strlen(data);
	}

	APPLOG_DBG("writerId=%s, seqNo=%ld, len=%ld", writerId, seqNo, len);

	(void) ::gettimeofday(&nowT, 0);

	fio_.start_append(data, len, ctxt);
	fio_.finish_append(ctxt);

	AuditLog::Entry e(writerId);

	e.startT = nowT;
	e.seqNo = seqNo;
	e.offset = ctxt.getOffset();
	e.len = len;
	e.type = AuditLog::appendRecord;
	e.checksumType = type;
	if (sum == (clusterio::checksum_t *) 0)
	{
		::memset(e.checksum, '\0', sizeof(e.checksum));
	}
	else
	{
		int sz = sum->length();

		for (int i = 0; i < sz; i++)
		{
			e.checksum[i] = (*sum)[i];
		}
	}

	auditor_.write(e);

	MutexGuard guard(mtx_);
//	if (goodRecords_.head != (DLList::Node *) 0)
	{
		adjRecords(e);
	}

	APPLOG_DBG("FH::append end wid=%s", writerId);
}


size_t
FileHandler::start_read(FileIO::ReadContext &ctxt, size_t offset, size_t len) throw (OpenException, ReadException)
{
	Counter ctr(semCtr_);
	size_t retVal = 0;
	size_t bufOffset = 0;

	APPLOG_DBG("FH::start_read(%ld,%ld)", offset, len);

	loadAuditLogForRead();
	fixupGoodList();

	MutexGuard guard(mtx_);

	for (DLList::Node *n = goodRecords_.head; n != (DLList::Node *) 0; n = n->next)
	{
		RecordAdj *ra = reinterpret_cast<RecordAdj *>(n);

//		APPLOG_DBG("node log=%ld, phys=%ld, len=%ld", ra->logOffset, ra->entry.offset, ra->entry.len);

		if (offset + len < ra->logOffset)
		{
			APPLOG_DBG("current n->offset(%ld) past requested offset (%ld) + len(%ld)", ra->logOffset, offset, len);
			; //@@TODO: error
		}
		else if (ra->logOffset + ra->entry.len < offset)
		{
//			APPLOG_DBG("Skipping past offset=%ld len=%ld", ra->logOffset, ra->entry.len);
			; // we want to read past this
		}
		else if (ra->logOffset <= offset)
		{
			size_t delta = offset - ra->logOffset;
			size_t newLen = min(len, ra->entry.len - delta);

			APPLOG_DBG("starting read delta=%ld, phys=%ld len=%ld, bufOffset=%ld",
				delta, ra->entry.offset + delta, newLen, bufOffset);

			fio_.start_read(ctxt, ra->entry.offset + delta, newLen, bufOffset);
			retVal += newLen;
			bufOffset += newLen;
			offset += newLen;
			len -= newLen;
		}
		if (len == 0)
		{
			break;
		}
	}

	APPLOG_DBG("FH::start_read returning %ld", retVal);

	return retVal;
}


size_t
FileHandler::finish_read(FileIO::ReadContext &ctxt, size_t offset)
	throw (rws::ReadException)
{
	Counter ctr(semCtr_);

	fio_.finish_read(ctxt);

	size_t retVal = ctxt.getActualLen();

	APPLOG_DBG("finish_read actual=%ld", retVal);

	if (ctxt.getActualLen() == 0)
	{
		throw ReadException(NICE_LOC, Resource(fname_.get()), Args(offset, ctxt.getRequestedLen(), ctxt.getActualLen()), Status());
	}

	return retVal;
}


clusterio::length_t
FileHandler::undo_appends(const char *writerId, CORBA::ULong &nextSeqNum)
	throw (rws::OpenException, rws::WriteException, rws::BadDataException)
{
	clusterio::length_t retVal;
	Counter ctr(semCtr_);

	APPLOG_DBG("FH::undo_appends start file=%s, wid=%s", fname_.get(), writerId);

	loadAuditLogForRead();

	retVal = undo(writerId, 0, 0);

	{
		MutexGuard guard(mtx_);
		WriterInfo *wi = findWriterInfo(writerId);

		nextSeqNum = 1;
		if (wi != (WriterInfo *) 0)
		{
			nextSeqNum += wi->seqNo;
		}
	}

	APPLOG_DBG("FH::undo_appends end file=%s, wid=%s", fname_.get(), writerId);

	return retVal;
}


clusterio::length_t
FileHandler::undo(const char *writerId, size_t offset, size_t len) throw (OpenException, WriteException)
{
	clusterio::length_t retVal;
	AuditLog::Entry e(writerId);

	e.offset = offset;
	e.len = len;
	e.type = AuditLog::undoRecord;

	auditor_.write(e);

	{
		MutexGuard guard(mtx_);

		retVal = adjRecords(e);
	}

	return retVal;
}


void
FileHandler::setAppendToken(long long token) throw (rws::BadDataException)
{
	MutexGuard guard(mtx_);

	if ((appendToken_ == 0L) || (token >= appendToken_))
	{
		appendToken_ = token;
	}
	else
	{
		throw BadDataException(NICE_LOC, Resource(fname_.get()), Args(0,
			appendToken_, token), Status("Can't go backwards in appendToken"));
	}
}


void
FileHandler::copyFrom(clusterio::ReadWriteService_ptr rws, const clusterio::file_handle &sourcefd, const char *nic)
	throw (rws::ThreadException)
{
	MutexGuard guard(mtx_);

	ServerSocketReader *ssr[2];
	RunnablePtr sr[2];
	ptr<ThreadRunnable> tr[2];

	ssr[0] = fio_.copyFrom(nic);
	sr[0] = ssr[0];

	ssr[1] = auditor_.copyFrom(nic);
	sr[1] = ssr[1];

	for (int i = 0; i < 2; i++)
	{
		time_t startT = ::time((time_t *) 0);

		tr[i] = new ThreadRunnable(sr[i]);
		tr[i].get()->start();

		while ((tr[i].get()->getRunState() == AsyncRunnable::notrunning) ||
			(ssr[i]->getServerPort() == 0))
		{
			if (time((time_t *) 0) - startT > 10)
			{
				throw ThreadException(NICE_LOC, Resource(fname_.get(), ssr[i]->getServerPort()), Args(i, tr[i].get()->getRunState(), AsyncRunnable::notrunning), Status("copyFrom thread failed to listen in time"));
			}
#ifdef __linux
			pthread_yield();
#else
			thr_yield();
#endif
		}
	}

	APPLOG_DBG("copyFrom(%s) fsp=%d, asp=%d", nic, ssr[0]->getServerPort(), ssr[1]->getServerPort());

	rws->system_cp(sourcefd, nic, ssr[0]->getServerPort(), ssr[1]->getServerPort());

	// Now wait for the secondary servers to finish their write.
	//
	for (int i = 0; i < 2; i++)
	{
		AsyncRunnable::erunstate thrState = tr[i]->wait();

		APPLOG_DBG("waited on secondary copyFrom[%d] state==%d", i, thrState);
	}
}


void
FileHandler::copyTo(const char *destHost, int destFilePort, int destAuditPort)
	throw (rws::Exception)
{
	MutexGuard guard(mtx_);

	APPLOG_DBG("Starting fio copyTo");
	fio_.copyTo(destHost, destFilePort);

	APPLOG_DBG("Starting auditor copyTo");
	auditor_.copyTo(destHost, destAuditPort);
}

void
FileHandler::rm() throw (CloseException, UnlinkException)
{
	Counter ctr(semCtr_);

	//@@TODO: Should we be looking at fwrap::isDirMissing() here?  A missing
	// basedir file here is ok, after all.
	//
	fio_.rm();
	auditor_.rm();
}


bool
FileHandler::isPresent() throw ()
{
	bool retVal = fio_.isPresent() && auditor_.isPresent();

	return retVal;
}


void
FileHandler::makeUnreachable() throw (FileModeException, CloseException)
{
	Counter ctr(semCtr_);

	fio_.makeUnreachable();
	auditor_.makeUnreachable();
}


FileHandler::WriterInfo *
FileHandler::findWriterInfo(const char *wid)
{
	WriterInfo *retVal = (WriterInfo *) 0;

	for (WriterRecords::iterator itor = widInfo_.find(wid); itor != widInfo_.end(); ++itor)
	{
		retVal = itor->second;
		break;
	}

	if (retVal == (WriterInfo *) 0)
	{
		retVal = new WriterInfo();
		retVal->wid = ptr_strdup(wid);
		retVal->seqNo = 0;
		retVal->recs = (WriterAdj *) 0;
		widInfo_[retVal->wid.get()] = retVal;
	}

	return retVal;
}


bool
FileHandler::checkWriterSeqNo(WriterInfo *widInfo, size_t seqNo)
{
	bool retVal = true;
	size_t priorV = widInfo->seqNo;

	FAIL_FH_IF("rhCheckSeqNo", return false)

	if (priorV < seqNo)
	{
		widInfo->seqNo = seqNo;
	}
	else
	{
		retVal = false;
	}

	APPLOG_DBG("checkWriterSeq(%s,%ld)=%d", widInfo->wid.get(), seqNo, retVal);

	return retVal;
}


bool
FileHandler::checkWriterSeqNo(const char *wid, size_t seqNo)
{
	WriterInfo *widInfo = findWriterInfo(wid);
	bool retVal = checkWriterSeqNo(widInfo, seqNo);

	return retVal;
}


clusterio::length_t
FileHandler::adjRecords(const AuditLog::Entry &e)
{
	clusterio::length_t retVal = 0;

	if (e.type == AuditLog::appendRecord)
	{
		RecordAdj *ra = new RecordAdj(e, trailingAdj_);
		WriterInfo *wi = findWriterInfo(e.writerId);

		APPLOG_DBG("adjRecord adding wid=%s phys=%ld log=%ld len=%ld, trailing=%ld",
			e.writerId, e.offset, ra->logOffset, e.len, trailingAdj_);
		checkWriterSeqNo(wi, e.seqNo);

		//@@TODO Need to make sure that ra is inserted in strictly increasing
		// order by disk offset, to account for the fact that audit entries
		// may not be appended in that order.
		//
		goodRecords_.append(ra);
//		dump();
		if (wi->recs == (WriterAdj *) 0)
		{
			APPLOG_DBG("create new recs for wid=%s", e.writerId);
			wi->recs = new list<RecordAdj *>();
		}
		wi->recs->push_front(ra);
	}
	else if (e.type == AuditLog::undoRecord)
	{
		WriterInfo *wi = findWriterInfo(e.writerId);

		if (wi->recs != (WriterAdj *) 0)
		{
			APPLOG_DBG("adjusting for wid=%s", e.writerId);
			for (WriterAdj::iterator itor = wi->recs->begin(); itor != wi->recs->end(); itor++)
			{
				RecordAdj *ra = *itor;

				if (ra->next == (RecordAdj *) 0)
				{
					APPLOG_DBG("increasing trailing (was %ld) by %ld", trailingAdj_, ra->pendingAdj);
					trailingAdj_ += ra->pendingAdj + ra->entry.len;
				}
				else
				{
					RecordAdj *nxt = reinterpret_cast<RecordAdj *>(ra->next);

					APPLOG_DBG("undoing for wid=%s pending=%ld, this len=%ld, next pending=%ld", e.writerId, ra->pendingAdj, ra->entry.len, nxt->pendingAdj);
					nxt->pendingAdj += ra->pendingAdj + ra->entry.len;
				}
				retVal += ra->entry.len;
				APPLOG_DBG("Undoing %ld/%ld", ra->entry.offset, ra->entry.len);
				goodRecords_.unlink(ra);
//				dump();
				delete ra;
				needFixup_ = true;
			}
//@@TODO			wi->recs->size(0);
			delete wi->recs;
			wi->recs = (WriterAdj *) 0;
		}
	}
	else
	{
		APPLOG_ALARM("Unknown AuditLog::recordType of %d", e.type);
	}

	return retVal;
}


void
FileHandler::fixupGoodList()
{
	MutexGuard guard(mtx_);

	if (needFixup_)
	{
		size_t delta = leadingAdj_;

		APPLOG_DBG("fixupGoodList: leadingAdj=%ld", delta);

		for (DLList::Node *n = goodRecords_.head; n != (DLList::Node *) 0; n = n->next)
		{
			RecordAdj *ra = reinterpret_cast<RecordAdj *>(n);

			delta += ra->pendingAdj;
			APPLOG_DBG("fixup: phy=%ld, log=%ld delta to apply=%ld => %ld", ra->entry.offset, ra->logOffset, delta, (ra->logOffset - delta));
			if (delta <= ra->logOffset)
			{
				ra->logOffset -= delta;
			}
			else
			{
				APPLOG_DBG("Error in fixup: delta is %ld, but logOffset=%ld", delta, ra->logOffset);
				ra->logOffset = 0;
			}
			ra->pendingAdj = 0;
		}

		trailingAdj_ += delta;
		needFixup_ = false;
	}
}


#if 0
void
FileHandler::dump()
{
	APPLOG_DBG("head=x%lx tail=%lx", goodRecords_.head, goodRecords_.tail);
	DLList::Node *n = goodRecords_.head;

	while (n != 0) {
		RecordAdj *ra = (RecordAdj *) n;

		APPLOG_DBG("\tthis=x%lx prev=x%lx next=x%lx offset=%ld len=%ld wid=%s", ra, ra->prev, ra->next, ra->entry.offset, ra->entry.len, ra->entry.writerId);
		n = n->next;
	}
}
#endif


void
FileHandler::dumpState(const char *what,
	clusterio::ReadWriteService::dump_format fmt,
	clusterio::ReadWriteService::detail_level lvl, ostream &oss)
{
	oss << "privateId=" << privateId_ << "\n";
	oss << "file=" << fname_.get() << "\n";
	oss << "appendToken=" << appendToken_ << "\n";
	oss << "usageCount=" << getCurrentUserCount() << "\n";

	if (lvl == clusterio::ReadWriteService::regular)
	{
		oss << "mode=" << mode_ << "\n";
	}

	if (lvl == clusterio::ReadWriteService::expanded)
	{
		oss << "leadingAdj=" << leadingAdj_ << "\n";
		oss << "trailingAdj=" << trailingAdj_ << "\n";
		oss << "needFixup=" << needFixup_ << "\n";
		oss << "numWriters=" << widInfo_.size() << "\n";
		oss << "numRecords=" << goodRecords_.length() << "\n";
	}
}
