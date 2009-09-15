
// ----------------------------------------------------------------------
//
// File:      LRUCache.h
// Author:    Steve Jackson
// Created:   Tue Mar 30 16:58:53 PST 2004 on dev1.looksmart.com
// Project:
// Purpose:
//
// Copyright (c) 2004 Steve Jackson.
//
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
//
// $Id: LRUCache.h,v 1.10 2004/07/16 19:16:02 sjackson Exp $


#ifndef LRUCACHE_H
#define LRUCACHE_H 1

#include "ptr_strdup.h"
#include <string.h>

#define NEED_HASH_MAP
#include "frame_config.h"

#include <ext/hash_map>
#include <iterator>
#include <list>
#include <limits.h>

#ifdef EMIT_CACHE_DEBUG
extern void
CACHE_DEBUG(const char *fmt, ...);
#else
#define CACHE_DEBUG(...)
#endif


// What is stored in the hashtable
//

template <class T>
class CacheEntry {
public:
	CacheEntry() {
		CACHE_DEBUG("CacheEntry.ctor\n");
	};

	CacheEntry(size_t sz, const char *s, T *e) : thisSize(sz), key(strdup_wnew(s)), entry(e) {
		CACHE_DEBUG("CacheEntry.ctor2\n");
	};

	CacheEntry(const CacheEntry &a) : thisSize(a.thisSize), key(a.key), entry(a.entry) {
		CACHE_DEBUG("CacheEntry.ctor\n");
	};

	~CacheEntry() {
		CACHE_DEBUG("CacheEntry.dtor\n");
		thisSize = 0;
		key = 0;
		entry = 0;
	 }
	const CacheEntry &operator=(const CacheEntry &a) {
		CACHE_DEBUG("CacheEntry.operator=\n");
		thisSize = a.thisSize;
		key = a.key;
		entry = a.entry;
	};

	size_t thisSize;
	char_ptr key;
	ptr<T> entry;
};


// How we test for equality in the hash buckets.
//
struct eqstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return keycmp(s1, s2);
	}

	static bool keycmp(const char *s1, const char *s2)
	{
		bool retVal = strcmp(s1, s2) == 0;

		CACHE_DEBUG("keycmp(%s,%s)=%d\n", s1, s2, (int) retVal);

		return retVal;
	}
};


// A templatized Least-Recently-Used Cache.  Each cache item has a "size".
// The cache has a maximum size; when that size is reached, cache entries are
// removed until the cumulative size is below the maximum.
//
//  Cache items are pointers to objects.
//
// Both the key and cache item are owned by the Cache; that is, they will be
// deleted when no longer used, or when the Cache object is destroyed.
//
template <class T>
class LRUCache {
public:

	LRUCache();
	LRUCache(int maxSize, bool autoPrune = true);
	virtual ~LRUCache();

	void setMaxSize(long maxSize);
	long getMaxSize() const;

	long getCurrentSize() const;

	void add(const char *key, T *item, size_t size);
	void remove(const char *key);
	void update(const char *key, T *newItem, size_t newSize);

	void touch(const char *key);

	void removeAll();

	T *getUntouched(const char *key);

	// This is now getUntouched() + touch()
	T *get(const char *key);
	
	char_ptr * getAllKeys(long &len) const;

	void rebuildCurrSize();

	void dumpUsage(FILE *);
	void dumpCache(FILE *);
protected:

	virtual bool isSafeToRemove(const T *) const;

private:
	bool autoPrune_;
	long maxSize_;
	long currSize_;

	typedef hash_map<const char *, CacheEntry<T> *, hash<const char *>, eqstr> CacheType;
	typedef list<const char *> UsageType;

	CacheType cache_;
	UsageType usage_;

	void prune();

	CacheEntry<T> *addCache(const char *key, T *item, size_t size);
	size_t dropCache(const char *key);

	const char *dropUsage(const char *key);
};



template <class T>
LRUCache<T>::LRUCache()
: autoPrune_(true), maxSize_(0), currSize_(0)
{
}


template <class T>
LRUCache<T>::LRUCache(int maxSize, bool autoPrune)
: autoPrune_(autoPrune), maxSize_((int) maxSize), currSize_(0)
{
	CACHE_DEBUG("ctor(%d) %ld/%ld\n", maxSize, usage_.size(), cache_.size());
}


template <class T>
LRUCache<T>::~LRUCache()
{
	CACHE_DEBUG("~LRUCache\n");

	removeAll();
	maxSize_ = 0L;
}


template <class T> long
LRUCache<T>::getCurrentSize() const
{
	return currSize_;
}


template <class T> long
LRUCache<T>::getMaxSize() const
{
	return maxSize_;
}


template <class T> void
LRUCache<T>::setMaxSize(long maxSize)
{
	maxSize_ = maxSize;
	if (autoPrune_)
	{
		prune();
	}
}


template <class T> void
LRUCache<T>::rebuildCurrSize()
{
	typename CacheType::iterator itor;
	long newSize = 0L;

	for (itor = cache_.begin(); itor != cache_.end(); itor++)
	{
		CacheEntry<T> *ce = itor->second;

		if (ce != (CacheEntry<T> *) 0)
		{
			newSize += ce->thisSize;
		}
	}

	CACHE_DEBUG("rebuildCurrSize from %ld to %ld\n", currSize_, newSize);

	currSize_ = newSize;
}


template <class T> void
LRUCache<T>::removeAll()
{
	CACHE_DEBUG("removeAll(usage=%ld,cache=%ld)\n", (long) usage_.size(),
		(long) cache_.size());

	usage_.clear();
	bool more = true;
	while (more)
	{
		more = false;
		for (typename CacheType::iterator itor = cache_.begin(); itor != cache_.end(); itor++)
		{
			CacheEntry<T> *ce = itor->second;

			cache_.erase(itor->first);
			if (ce != (CacheEntry<T> *) 0)
			{
				delete ce;
			}
			more = true;
			break;
		}
	}
	cache_.clear();
	currSize_ = 0L;
}


/**
Remove CacheEntries until the total size is <= maxSize.
*/

template <class T> void
LRUCache<T>::prune()
{
	bool rebuild = false;
#ifdef EMIT_CACHE_DEBUG
	long usSz = usage_.size();
	long caSz = cache_.size();
#endif


	while ((currSize_ > maxSize_) && (usage_.size() > 0))
	{
		const char *key = usage_.back();
		T *e = getUntouched(key);

		if (isSafeToRemove(e))
		{
			CACHE_DEBUG("Pruning(%d, %d, %ld, %ld) %s\n", usage_.size(), cache_.size(), currSize_, maxSize_, key);
			
			usage_.pop_back();
			size_t numDropped = dropCache(key);

			rebuild = rebuild || (numDropped != 1);
		}
		else
		{
			break;
		}
	}

	CACHE_DEBUG("pruned from usage=%ld/cache=%ld to usage=%ld/cache=%ld\n", usSz, caSz, usage_.size(),
		cache_.size());

	if (rebuild)
	{
		rebuildCurrSize();
	}
}


template <class T> void
LRUCache<T>::touch(const char *key)
{
	const char *realKey = dropUsage(key);

	if (realKey != (const char *) 0)
	{
		usage_.push_front(realKey);
	}
}


template <class T> CacheEntry<T> *
LRUCache<T>::addCache(const char *key, T *item, size_t size)
{
	CacheEntry<T> *retVal = new CacheEntry<T>(size, key, item);
	const char *realKey = retVal->key.get();

	cache_[realKey] = retVal;
	usage_.push_front(realKey);

	CACHE_DEBUG("addCache(%s,%lx)=%d/%d\n", key, (long) retVal, usage_.size(), cache_.size());

	currSize_ += size;

	return retVal;
}


template <class T> size_t
LRUCache<T>::dropCache(const char *key)
{
	size_t retVal = 0;
	CacheEntry<T> *item = (CacheEntry<T> *) 0;

	(void) dropUsage(key);
	for (typename CacheType::iterator itor = cache_.find(key); itor != cache_.end(); itor++)
	{
		item = itor->second;
		if (item != (CacheEntry<T> *) 0)
		{
			currSize_ -= item->thisSize;
			retVal++;
			CACHE_DEBUG("dropCache(%s)=%lx, %d/%d\n", key, (long) item, usage_.size(), cache_.size());
			break;
		} else {
			CACHE_DEBUG("dropCache(%s) is null!\n", key);
		}
	}

	cache_.erase(key);

	if (item != (CacheEntry<T> *) 0)
	{
		delete item;
	}

	return retVal;
}


template <class T> const char *
LRUCache<T>::dropUsage(const char *key)
{
	const char *retVal = (const char *) 0;

	for (UsageType::iterator itor = usage_.begin(); itor != usage_.end(); itor++)
	{
		if (eqstr::keycmp(key, *itor))
		{
			retVal = *itor;
			itor = usage_.erase(itor);
			
			break;
		}
	}

	CACHE_DEBUG("dropUsage(%s)=%d/%d\n", key, usage_.size(), cache_.size());

	return retVal;
}


template <class T> void
LRUCache<T>::add(const char *key, T *item, size_t size)
{
	CACHE_DEBUG("::add %s\n", key);

	CACHE_DEBUG("pre remove %ld/%ld\n", usage_.size(), cache_.size());

	remove(key);

	CACHE_DEBUG("post remove %ld/%ld\n", usage_.size(), cache_.size());

	currSize_ += size;
	if (autoPrune_)
	{
		prune();
	}
	CACHE_DEBUG("post prune %ld/%ld\n", usage_.size(), cache_.size());
	currSize_ -= size;

	(void) addCache(key, item, size);
	CACHE_DEBUG("post add* %ld/%ld\n", usage_.size(), cache_.size());
}


template <class T> void
LRUCache<T>::remove(const char *key)
{
	CACHE_DEBUG("::remove(%s)\n", key);
	size_t numDC = dropCache(key);

	if (numDC > 1)
	{
		CACHE_DEBUG("Dropped too many (%d); rebuilding\n", numDC);
		rebuildCurrSize();
	}
	else
	{
	}
}


template <class T> T *
LRUCache<T>::getUntouched(const char *key)
{
	T *retVal = (T *) 0;

	for (typename CacheType::iterator itor = cache_.find(key); itor != cache_.end(); itor++)
	{
		CacheEntry<T> *entry = itor->second;

		if (entry != (CacheEntry<T> *) 0)
		{
			retVal = entry->entry.get();

			CACHE_DEBUG("::getUntouched(%s)=%s/%lx, %lx\n", key, entry->key.get(), (long) entry->key.get(), (long) retVal);
			break;
		}
		else
		{
			CACHE_DEBUG("::getUntouched(%s) found null!\n", key);
		}
	}

	return retVal;
}



template <class T> T *
LRUCache<T>::get(const char *key)
{
	T *retVal = getUntouched(key);

	if (retVal != (T *) 0)
	{
		(void) touch(key);
	}

	return retVal;
}


template <class T> void
LRUCache<T>::dumpUsage(FILE *fh)
{
	int i = 0;

	for (UsageType::iterator itor = usage_.begin(); itor != usage_.end(); itor++)
	{
		const char *s = *itor;
		fprintf(fh, "usage[%d] key=%s, addr=%p\n", i++, s, (void *) s);
	}
}


template <class T> void
LRUCache<T>::dumpCache(FILE *fh)
{
	int i = 0;

	for (typename CacheType::iterator itor = cache_.begin(); itor != cache_.end(); itor++)
	{
		const char *s = itor->first;
		CacheEntry<T> *ce = itor->second;
		fprintf(fh, "cache[%d] key=%s, addr=%p entry addr=%p\n", i++, s,
			(void *) s, ce->entry.get());
	}
}


template <class T> bool
LRUCache<T>::isSafeToRemove(const T *) const
{
	return true;
};


template <class T> char_ptr *
LRUCache<T>::getAllKeys(long &len) const
{
	len = usage_.size();
	char_ptr *retVal = new char_ptr[len];
	LRUCache<T> *t = const_cast<LRUCache<T> *>(this);
	int cnt = 0;

	for (UsageType::iterator itor = t->usage_.begin(); itor != t->usage_.end(); itor++)
	{
		const char *key = *itor;

		retVal[cnt++] = ptr_strdup(key);
	}

	return retVal;
};

#endif
