
#include "RWSExceptions.h"
#include <string>
#include <sstream>


const char *rws::Exception::CLASS = "generic";
const char *rws::OpenException::CLASS = "open";
const char *rws::ReadException::CLASS = "read";
const char *rws::WriteException::CLASS = "write";
const char *rws::CopyException::CLASS = "copy";
const char *rws::SpaceException::CLASS = "space";
const char *rws::CloseException::CLASS = "close";
const char *rws::UnlinkException::CLASS = "unlink";
const char *rws::BadDataException::CLASS = "baddata";
const char *rws::StaleFileException::CLASS = "stalefile";
const char *rws::FileModeException::CLASS = "filemode";
const char *rws::ThreadException::CLASS = "thread";


std::ostream &
rws::operator<<(std::ostream &os, const rws::Location &l)
{
	if (l.src_ != (const char *) 0)
	{
		os << ' ' << l.src_;
		if (l.line_ != -1)
		{
			os << ':';
		}
	}

	if (l.line_ != -1)
	{
		os << l.line_;
	}

	if (l.ctxt_ != (const char *) 0)
	{
		os << " (" << l.ctxt_ << ")";
	}

	return os;
}


std::ostream &
rws::operator<<(std::ostream &os, const rws::Resource &r)
{
	if (r.fname_ != (const char *) 0)
	{
		os << " file=" << r.fname_;

		if ((r.fd1_ != -1) || (r.fd2_ != -1))
		{
			os << ',';
		}
	}

	if (r.fd1_ != -1)
	{
		os << " fd1=" << r.fd1_;
		if (r.fd2_ != -1)
		{
			os << ',';
		}
	}
	if (r.fd2_ != -1)
	{
		os << " fd2=" << r.fd2_;
	}

	return os;
}


std::ostream &
rws::operator<<(std::ostream &os, const rws::Args &a)
{
	if (a.str_ != (const char *) 0)
	{
		os << " str=" << a.str_;
	}

	if (a.offset_ != -1)
	{
		os << " off=" << a.offset_;
	}

	if (a.req_ != -1)
	{
		os << " reqested=" << a.req_;
	}

	if (a.act_ != -1)
	{
		os << " actual=" << a.act_;
	}

	return os;
}


std::ostream &
rws::operator<<(std::ostream &os, const rws::Status &s)
{
	if (s.errno_ != 0)
	{
		os << " errno=" << s.errno_;
	}

	if (s.msg_ != (const char *) 0)
	{
		os << " msg=" << s.msg_;
	}

	return os;
}


std::ostream &
rws::operator<<(std::ostream &os, const rws::Exception &e)
{
	os << e.l_ << e.r_ << e.a_ << e.s_;

	return os;
}


const char *
rws::Exception::what() const throw ()
{
	if (message_[0] == '\0')
	{
		std::ostringstream oss;

		try {
			oss << *this;

			std::string s = oss.str();
			const char *p = s.c_str();

			if (p != (const char *) 0)
			{
				size_t len = strlen(p);

				if (len > BE_LEN)
					len = BE_LEN;

				::strncpy(message_, p, len);
			}
		} catch (...) {
		}
	}

	return BaseException::what();
}

