
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "AppLog.h"
#include "netwrap.h"


long long
netwrap::htonll(long long l)
{
	long long retVal = l;

#ifdef __linux
	if (1 == htons(1))
	{
		retVal = l;
	}
	else
	{
		union {
			long long l;
			unsigned char b[sizeof(long long)];
		} flip;
		size_t sz = sizeof(flip.b);
		size_t half = sz >> 1;

		flip.l = l;
		size_t end = sz - 1;

		for (size_t i = 0; i < half; i++, end--)
		{
			unsigned char tmp = flip.b[i];

			flip.b[i] = flip.b[end];
			flip.b[end] = tmp;
		}

		retVal = flip.l;
	}
#endif
	
	return retVal;
}


int
netwrap::createServer(int &port, const char *host)
{
	int retVal;
	struct sockaddr_in addr;

	(void) ::memset(&addr, '\0', sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);

if (host != (const char *) 0)
	{
		int ok = getIpAddr(host, &addr.sin_addr.s_addr);

		if (ok != 0)
		{
			APPLOG_WARN("failure to get IP for %s, errno=%d", host, errno);
			return -3;
		}
	}
	else
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	retVal = ::socket(PF_INET, SOCK_STREAM, 0);

	if (retVal == -1)
	{
		APPLOG_WARN("socket() failure: errno=%d", errno);
	}
	else
	{
		socklen_t sz = sizeof(addr);
		int b = ::bind(retVal, (struct sockaddr *) &addr, sz);

		if (b == -1)
		{
			APPLOG_WARN("bind failure(%d) for %s:%d errno=%d", retVal, host, port, errno);
			closeServer(retVal);
			retVal = -2;
		}
		else
		{
			//@@TODO What options should we set, if any?
			// int yes = 1;
			// ::setsockopt(retVal, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

			if (port == 0)
			{
				struct sockaddr_in addr2;
				socklen_t sz = sizeof(addr2);
				int ok = getsockname(retVal, (struct sockaddr *) &addr2, &sz);

				if (ok == -1)
				{
					APPLOG_WARN("failed to find port server %s is bound to.  errno=%d", host, errno);
				}
				else
				{
					port = ntohs(addr2.sin_port);

					APPLOG_INFO("server %s bound to %d", host, port);
				}
			}
		}
	}

	return retVal;
}


int
netwrap::waitForClient(int sock, struct sockaddr *addr, int backlog)
{
	int retVal;
	int lStat = ::listen(sock, backlog);

	if (lStat == -1)
	{
		APPLOG_WARN("listen(%d) failed; errno=%d", sock, errno);
		retVal = -1;
	}
	else
	{
		socklen_t sz = sizeof(struct sockaddr);
		retVal = ::accept(sock, addr, &sz);

		if (retVal == -1)
		{
			APPLOG_WARN("accept(%d) failed; errno=%d", sock, errno);
			retVal = -2;
		}
	}

	return retVal;
}


int
netwrap::connectToServer(const char *host, int port)
{
	int retVal;
	struct sockaddr_in addr;

	(void) ::memset(&addr, '\0', sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);

	int isInetAddr = ::inet_pton(PF_INET, host, &(addr.sin_addr.s_addr));

	if (isInetAddr <= 0)
	{
		int ok = getIpAddr(host, &addr.sin_addr.s_addr);

		if (ok != 0)
		{
			APPLOG_WARN("unknown host: %s.  Port=%d errno=%d", host, port, errno);
			return -1;
		}
	}

	retVal = ::socket(PF_INET, SOCK_STREAM, 0);

	if (retVal == -1)
	{
		APPLOG_DBG("socket(%s,%d)=%d, errno=%d", host, port, retVal, errno);
		return -2;
	}

	int connStat = ::connect(retVal, (struct sockaddr *) &addr, sizeof(addr));

	if (connStat == -1)
	{
		APPLOG_DBG("connect(%s, %d, %d)=%d, errno=%d", host, port, retVal, connStat, errno);
		return -3;
	}

	return retVal;
}


int
netwrap::getIpAddr(const char *host, in_addr_t *addr, int which)
{
	int retVal = 0;

	(void) ::memset(addr, '\0', sizeof(struct in_addr));

	if ((host != (const char *) 0) && (which >= 0))
	{
		struct hostent hostStruct;
		struct hostent *result = (struct hostent *) 0;
		char buf[4096];
		int localErrno;

#if __linux
		int found = ::gethostbyname_r(host, &hostStruct, buf, sizeof(buf), &result, &localErrno);

		if ((found == 0) && (result != (struct hostent *) 0))
		{
			char **p = result->h_addr_list;
			int cnt = 0;

			while ((cnt < which) && (*p != (char *) 0))
			{
				p++;
				cnt++;
			}
				
			if (*p != (char *) 0)
			{
				in_addr_t *a = (in_addr_t *) result->h_addr_list[which];

				(void) ::memcpy(addr, a, sizeof(struct in_addr));
			}
			else
			{
				APPLOG_WARN("Not enough aliases (%d)", which);
				retVal = -3;
			}
		}
		else
		{
			APPLOG_WARN("gethostbyname_r(%s) failed; errno=%d local=%d", host, errno, localErrno);
			retVal = -2;
		}
#endif
	}
	else
	{
		APPLOG_WARN("null host or bad alias choice: %d", which);
		retVal = -1;
	}

	return retVal;
}


int
netwrap::closeServer(int sock)
{
	int retVal = ::close(sock);

	if (retVal == -1)
		APPLOG_WARN("close(%d) failed; errno=%d", sock, errno);

	return retVal;
}


int
netwrap::closeClient(int sock)
{
	int retVal = ::close(sock);

	if (retVal == -1)
		APPLOG_WARN("close(%d) failed; errno=%d", sock, errno);

	return retVal;
}
