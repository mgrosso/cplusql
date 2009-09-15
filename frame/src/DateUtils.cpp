
#include "DateUtils.h"


long long DateUtils::MICRO_SECONDS = 1000000L;


void
DateUtils::getTime(struct timeval &tv) throw ()
{
#ifdef FRAME_IS_MISSING_GETTIMEOFDAY
	(void) ::time(&tv.tv_sec);
	tv.tv_usec = 0;
#else
	(void) ::gettimeofday(&tv, 0);
#endif
}


void
DateUtils::diffTime(const struct timeval &startT, const struct timeval *endT,
	struct timeval &diff) throw ()
{
	struct timeval eT;

	if (endT == (const struct timeval *) 0)
	{
		getTime(eT);
		endT = &eT;
	}

	long long s = startT.tv_sec;
	long long e = endT->tv_sec;

	s = s * MICRO_SECONDS + startT.tv_usec;
	e = e * MICRO_SECONDS + endT->tv_usec;

	long long df = e - s;
	diff.tv_sec = df / MICRO_SECONDS;
	diff.tv_usec = df % MICRO_SECONDS;
}


double
DateUtils::diffTime(const struct timeval &startT, const struct timeval *endT)
	throw ()
{
	struct timeval eT;

	if (endT == (const struct timeval *) 0)
	{
		getTime(eT);
		endT = &eT;
	}

	long long s = startT.tv_sec;
	long long e = endT->tv_sec;

	s = s * MICRO_SECONDS + startT.tv_usec;
	e = e * MICRO_SECONDS + endT->tv_usec;

	long long df = e - s;
	double retVal = (df + 0.0) / (MICRO_SECONDS + 0.0);

	return retVal;
}

