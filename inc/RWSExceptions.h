
#ifndef RWSEXCEPTIONS_H
#define RWSEXCEPTIONS_H 1

#include <errno.h>
#include <iostream>

#include "BaseException.h"


/**
A flexible way to capture most of the information needed to report on I/O
errors/exception.  The exceptions themselves are subclassed from Exception
so that catch() blocks can properly differentiate between the various types.

The Exception class itself has 4 parts:
* Location: where programatically/lexically it was thrown,
* Resource: what resource was involved (filename and/or fd),
* Args: what were the args (actual vs expected),
* Status: errno

There is also a virtual getClass() method for obtaining a mnemonic of the class.
For speed and correctness, the above 4 parts are included as objects in the
base Exception class (meaning no subclasses).

Here is an example usage:

throw OpenException(SRC_LOC, Resource(fname_), Status(errno));
throw WriteException(NICE_LOC, Resource(fname_, fdWr_), Args(-1, sz, numWr), Status(errno));
throw ReadException(ERR_CTXT("reader"), Resource(fname_, fdRd_), Args(offset, sz, numRd), Status(errno));

*/


namespace rws {

class Location {
public:
	inline Location() throw () : src_(0), line_(-1), ctxt_(0) {};
	inline Location(const char *s, int l, const char *c)
		throw () : src_(s), line_(l), ctxt_(c) {};
	inline Location(const Location &l)
		throw () : src_(l.src_), line_(l.line_), ctxt_(l.ctxt_) {};

	inline Location &operator=(const Location &l) throw ()
		{ src_ = l.src_; line_ = l.line_; ctxt_ = l.ctxt_; return *this; };

	friend std::ostream &operator<<(std::ostream &, const Location &);

private:

	const char *src_;
	int line_;
	const char *ctxt_;

};

#define ERR_CTXT(ctxt) rws::Location(__FILE__, __LINE__, ctxt)
#define SRC_LOC rws::Location(__FILE__, __LINE__, __func__)
#define NICE_LOC rws::Location(__FILE__, __LINE__, __PRETTY_FUNCTION__)


class Resource {
public:
	inline Resource(int fd1 = -1, int fd2 = -1) throw () : fname_(0), fd1_(fd1), fd2_(fd2) {};
	inline Resource(const char *fn, int fd1 = -1, int fd2 = -1) throw () : fname_(fn), fd1_(fd1), fd2_(fd2) {};
	inline Resource(const Resource &r) throw () : fname_(r.fname_), fd1_(r.fd1_), fd2_(r.fd2_) {};

	inline Resource &operator=(const Resource &r) throw ()
	{ fname_ = r.fname_; fd1_ = r.fd1_; fd2_ = r.fd2_; return *this; };

	friend std::ostream &operator<<(std::ostream &, const Resource &);

private:
	const char *fname_;
	int fd1_;
	int fd2_;
};


class Args {
public:
	inline Args(ssize_t off = -1, long long req = -1, long long act = -1) throw () : str_(0), offset_(off), req_(req), act_(act) {};
	inline Args(bool dummy, const char *str, ssize_t off = -1, long long req = -1, long long act = -1) throw () : str_(str), offset_(off), req_(req), act_(act) {};
	inline Args(const Args &a) throw () : str_(a.str_), offset_(a.offset_), req_(a.req_), act_(a.act_) {};

	inline Args &operator=(const Args &a) throw () { str_ = a.str_; offset_ = a.offset_; req_ = a.req_; act_ = a.act_;  return *this; };

	friend std::ostream &operator<<(std::ostream &, const Args &);

private:
	const char *str_;
	ssize_t offset_;
	long long req_;
	long long act_;

};


class Status {
public:
	inline Status() throw () : errno_(0), msg_(0) {};
	inline Status(int en, const char *msg = 0) throw () : errno_(en), msg_(msg) {};
	inline Status(const char *msg) throw () : errno_(0), msg_(msg) {};
	inline Status(const Status &s) throw () : errno_(s.errno_), msg_(s.msg_) {};

	inline Status &operator=(const Status &s) throw ()
	{ errno_ = s.errno_; msg_ = s.msg_;  return *this; };

	inline int getErrno() const throw () { return errno_; };
	inline const char * getMsg() const throw () { return msg_; };

	friend std::ostream &operator<<(std::ostream &, const Status &);

private:
	int errno_;
	const char *msg_;
};


class Exception: public BaseException {
public:
	static const char *CLASS;

	inline Exception(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : BaseException(""), l_(l), r_(r), a_(a), s_(s) {};
	inline Exception(const Location &l, const Resource &r, const Status &s)
		throw () : BaseException(""), l_(l), r_(r), s_(s) {};
	inline Exception(const Exception &e)
		throw () : BaseException(e), l_(e.l_), r_(e.r_), a_(e.a_), s_(e.s_) {};
	inline virtual ~Exception() throw () {};

	inline Exception &operator=(const Exception &e) throw ()
	{ BaseException::operator=(e); l_ = e.l_; r_ = e.r_; a_ = e.a_; s_ = e.s_; return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
	inline const Location &getLocation() const throw () { return l_; };
	inline const Resource &getResource() const throw () { return r_; };
	inline const Args &getArgs() const throw () { return a_; };
	inline const Status &getStatus() const throw () { return s_; };
	
	friend std::ostream &operator<<(std::ostream &, const Exception &e);

	const char *what() const throw();

private:

	Location l_;
	Resource r_;
	Args a_;
	Status s_;
};



class OpenException: public Exception {
public:
	static const char *CLASS;

	inline OpenException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw() : Exception(l, r, a, s) {};
	inline OpenException(const Location &l, const Resource &r, const Status &s)
		throw() : Exception(l, r, s) {};
	inline OpenException(const OpenException &e) throw() : Exception(e) {};
	inline virtual ~OpenException() throw() {};

	inline OpenException &operator=(const OpenException &oe) throw ()
	{ Exception::operator=(oe); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class ReadException: public Exception {
public:
	static const char *CLASS;

	inline ReadException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : Exception(l, r, a, s) {};
	inline ReadException(const Location &l, const Resource &r, const Status &s)
		throw () : Exception(l, r, s) {};
	inline ReadException(const ReadException &re) throw () : Exception(re) {};
	inline virtual ~ReadException() throw () {};

	inline ReadException &operator=(const ReadException re) throw ()
	{ Exception::operator=(re); throw *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class WriteException: public Exception {
public:
	static const char *CLASS;

	inline WriteException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw() : Exception(l, r, a, s) {};
	inline WriteException(const Location &l, const Resource &r, const Status &s)
		throw() : Exception(l, r, s) {};
	inline WriteException(const WriteException &we) throw () : Exception(we) {};
	inline virtual ~WriteException() throw () {};

	inline WriteException &operator=(const WriteException &we) throw()
	{ Exception::operator=(we); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class CopyException: public Exception {
public:
	static const char *CLASS;

	inline CopyException(const Location &l, const Resource &r, const Args &a, const Status &s)
		: Exception(l, r, a, s) {};
	inline CopyException(const Location &l, const Resource &r, const Status &s)
		: Exception(l, r, s) {};
	inline CopyException(const CopyException &ce) throw () : Exception(ce) {};
	inline virtual ~CopyException() throw () {};

	inline CopyException &operator=(const CopyException &ce) throw()
	{ Exception::operator=(ce); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class SpaceException: public WriteException {
public:
	static const char *CLASS;

	inline SpaceException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : WriteException(l, r, a, s) {};
	inline SpaceException(const Location &l, const Resource &r, const Status &s)
		throw () : WriteException(l, r, s) {};
	inline SpaceException(const SpaceException &se) throw () : WriteException(se) {};

	inline SpaceException &operator=(const SpaceException &se) throw ()
	{ WriteException::operator=(se); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class CloseException: public Exception {
public:
	static const char *CLASS;

	inline CloseException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : Exception(l, r, a, s) {};
	inline CloseException(const Location &l, const Resource &r, const Status &s)
		throw() : Exception(l, r, s) {};
	inline CloseException(const CloseException &ce) throw () : Exception(ce) {};
	inline virtual ~CloseException() throw () {};

	inline CloseException &operator=(const CloseException &ce) throw ()
	{ Exception::operator=(ce); return *this;};

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class UnlinkException: public Exception {
public:
	static const char *CLASS;

	inline UnlinkException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : Exception(l, r, a, s) {};
	inline UnlinkException(const Location &l, const Resource &r, const Status &s)
		throw () : Exception(l, r, s) {};
	inline UnlinkException(const UnlinkException &ue) throw () : Exception(ue) {};
	inline virtual ~UnlinkException() throw () {};

	inline UnlinkException &operator=(const UnlinkException &ue) throw ()
	{ Exception::operator=(ue); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class BadDataException: public Exception {
public:
	static const char *CLASS;

	inline BadDataException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : Exception(l, r, a, s) {};
	inline BadDataException(const Location &l, const Resource &r, const Status &s)
		throw () : Exception(l, r, s) {};
	inline BadDataException(const BadDataException &bde) throw () : Exception(bde) {};
	inline virtual ~BadDataException() throw () {};

	inline BadDataException &operator=(const BadDataException &bde) throw ()
	{ Exception::operator=(bde); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class StaleFileException: public Exception {
public:
	static const char *CLASS;

	inline StaleFileException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : Exception(l, r, a, s) {};
	inline StaleFileException(const Location &l, const Resource &r, const Status &s)
		throw () : Exception(l, r, s) {};
	inline StaleFileException(const StaleFileException &bde) throw () : Exception(bde) {};
	inline virtual ~StaleFileException() throw () {};

	inline StaleFileException &operator=(const StaleFileException &bde) throw ()
	{ Exception::operator=(bde); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};


class FileModeException: public Exception {
public:
	static const char *CLASS;

	inline FileModeException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : Exception(l, r, a, s) {};
	inline FileModeException(const Location &l, const Resource &r, const Status &s)
		throw () : Exception(l, r, s) {};
	inline FileModeException(const FileModeException &fme) throw () : Exception(fme) {};
	inline virtual ~FileModeException() throw () {};

	inline FileModeException &operator=(const FileModeException &fme) throw ()
	{ Exception::operator=(fme); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};



class ThreadException: public Exception {
public:
	static const char *CLASS;

	inline ThreadException(const Location &l, const Resource &r, const Args &a, const Status &s)
		throw () : Exception(l, r, a, s) {};
	inline ThreadException(const Location &l, const Resource &r, const Status &s)
		throw () : Exception(l, r, s) {};
	inline ThreadException(const ThreadException &fme) throw () : Exception(fme) {};
	inline virtual ~ThreadException() throw () {};

	inline ThreadException &operator=(const ThreadException &fme) throw ()
	{ Exception::operator=(fme); return *this; };

	inline virtual const char *getClass() const throw () { return CLASS; };
};



}; // namespace rws

#endif
