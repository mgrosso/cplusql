
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "FileHandler.h"
#include "RWSExceptions.h"
#include "ReadWriteController.h"

int main(int argc, char *argv[])
{
	char arg;
	char *fname = (char *) 0;
	long seqNo = 0;
	char *baseDir = strdup(".");
	char *strToAppend = (char *) 0;
	char *readCriteria = (char *) 0;
	char *undoCriteria = (char *) 0;
	char *writerId = (char *) 0;

	while ((arg = getopt(argc, argv, "a:b:f:r:s:u:w:")) != -1) {
		switch (arg) {
		case 'a': // append
			strToAppend = strdup(optarg);
			break;

		case 'b': // baseDir
			free(baseDir);
			baseDir = strdup(optarg);
			break;

		case 'f': // filename
			fname = strdup(optarg);
			break;

		case 'r': // read
			readCriteria = strdup(optarg);
			break;

		case 's': // seqNo
			seqNo = atoi(optarg);
			break;

		case 'u': // undo
			undoCriteria = strdup(optarg);
			break;

		case 'w': // writerId
			writerId = strdup(optarg);
			break;

		default:
			;
		}
	}

	Config cfg;
	//@@TODO
	ReadWriteController rwc(cfg);

	if (fname == (char *) 0) {
		printf("Need -f <filename>");
		exit(1);
	}

	if (writerId == (char *) 0) {
		printf("Need -w <writerId>");
		exit(1);
	}

	FileHandler *fh = rwc.getFileHandler(0, fname);

	if (strToAppend != (char *) 0)
	{
		try {
			fh->append(writerId, seqNo, strToAppend, strlen(strToAppend), clusterio::none, (clusterio::checksum_t *) 0);
		} catch (rws::Exception &e) {
			const char *s = e.what();

			printf("Caught a %s: %s\n", e.getClass(), s);
		}
	} else if (readCriteria != (char *) 0) {
		if (strcmp(readCriteria, "records") == 0) {
			AuditLog &al = fh->getAuditLog();
			AuditLog::ReadContext alCtxt;
	
			printf("Reading all records:\n");
			while (al.readNext(alCtxt)) {
				AuditLog::Entry &e = alCtxt.getEntry();
				printf("time=%lld, offset=%ld, len=%ld, type=%c writerId=%s\n", e.startT, e.offset, e.len, e.type, e.writerId);
				if (e.type == AuditLog::undoRecord)
				{
					printf("\tundo record\n");
				}
				else
				{
					FileIO &fio = fh->getFileIO();
					FileIO::ReadContext ctxt;

					try {
						fio.start_read(ctxt, e.offset, e.len, 0);
						fio.finish_read(ctxt);
					} catch (rws::Exception &e) {
						const char *s = e.what();

						printf("Caught a %s: %s\n", e.getClass(), s);
					}

					char *buf = ctxt.takeBuffer();

					printf("ReadContext.buffer = %s\n", buf);
					printf("ReqLen=%d, actualLen=%d\n", ctxt.getRequestedLen(),
						ctxt.getActualLen());
					delete [] buf;
				}
			}
		} else {
			char *dash = strchr(readCriteria, '-');

			if (dash != (char *) 0) {
				*dash = '\0';
				long start = atol(readCriteria);
				long len = atol(++dash);
				char buf[len + 2];

				memset(buf, '\0', len + 2);
				try {
					FileIO::ReadContext ctxt(buf, len, false);

					fh->start_read(ctxt, start, len);
					fh->finish_read(ctxt, start);
					printf("read=%s\n", buf);
				} catch (rws::Exception &e) {
					const char *s = e.what();

					printf("Caught a %s: %s\n", e.getClass(), s);
				}
			}
		}
	} else if (undoCriteria != (char *) 0) {
		char *dash = strchr(undoCriteria, '-');

		if (dash != (char *) 0)
		{
			*dash = '\0';
			long start = atol(undoCriteria);
			long len = atol(++dash);

			try {
				fh->undo(writerId, start, len);
			} catch (rws::Exception &e) {
				const char *s = e.what();

				printf("Caught a %s: %s\n", e.getClass(), s);
			}
		}
		else
		{
			//@@TODO
		}
	}

	return 0;
}
