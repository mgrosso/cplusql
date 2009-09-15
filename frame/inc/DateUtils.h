
#ifndef DATEUTILS_H
#define DATEUTILS_h 1

#include <sys/time.h>


namespace DateUtils {

	extern long long MICRO_SECONDS;

	void getTime(struct timeval &tv) throw ();
	void diffTime(const struct timeval &startT, const struct timeval *endT,
		struct timeval &diff) throw ();
	double diffTime(const struct timeval &startT, const struct timeval *endT = (const struct timeval *) 0)
		throw ();
};

#endif
