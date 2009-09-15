#include "LRUCache.h"
#include <stdio.h>
#include <cstdarg>

bool LRUCache_debugEnable = true;

#ifdef EMIT_CACHE_DEBUG
void
CACHE_DEBUG(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

	if (LRUCache_debugEnable)
	{
    	vfprintf(stderr, fmt, ap);
	}

    va_end(ap);
}
#endif
