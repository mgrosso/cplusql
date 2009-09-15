#include "LRUCache.h"
#include <stdio.h>


extern bool LRUCache_debugEnable;

class A {

public:
	A() : a_(-1) {
		CACHE_DEBUG("A::ctor\n");
	};
	A(int a) : a_(a) {
		CACHE_DEBUG("A::ctor %d\n", a);
	};
	A(const A &a) : a_(a.a_) {
		CACHE_DEBUG("A::copy %d\n", a_);
	};
	~A() {
		CACHE_DEBUG("A::dtor %d\n", a_);
	}

	const A &operator=(const A &a) {
		CACHE_DEBUG("A::operator= %d\n", a.a_);
		a_ = a.a_;
		return *this;
	}
private:
	int a_;
};



void fred(char_ptr p) {
	fprintf(stderr, "fred=%s\n", p.get());
}

int main(int argc, char *argv[])
{
	int sz = 18;

	if (argc > 1)
	{
		sz = atoi(argv[1]);
	}
	if (argc > 2)
	{
		LRUCache_debugEnable = false;
	}

	{
	LRUCache<const char> c(sz);

	c.add("a", strdup("a"), 1);
	c.add("b", strdup("b"), 18);
	c.add("ba", strdup("ba"), 1);
	c.add("new", strdup("new"), 3);
	c.add("a", strdup("new"), 3);

	c.dumpUsage(stderr);
	fprintf(stderr, "touching b\n");
	c.touch("b");
	fprintf(stderr, "\n");
	c.dumpUsage(stderr);

	const char *s = c.get("a");
	fprintf(stderr,"a: Got %s\n", (s == 0) ? "null" : s);

	s = c.get("b");
	fprintf(stderr,"b: Got %s\n", (s == 0) ? "null" : s);

	s = c.get("ba");
	fprintf(stderr,"ba: Got %s\n", (s == 0) ? "null" : s);

	s = c.get("new");
	fprintf(stderr,"new: Got %s\n", (s == 0) ? "null" : s);

	fprintf(stderr, "Going out of scope\n");
	}

	fprintf(stderr, "---------------\n");

	{
		LRUCache<A> aCache(sizeof(A)*3);

		aCache.add("a2", new A(2), sizeof(A));
		aCache.add("a2", new A(3), sizeof(A));
		aCache.add("a3", new A(4), sizeof(A));
		aCache.add("a3", new A(5), sizeof(A));
		aCache.add("a4", new A(6), sizeof(A));
		aCache.add("a5", new A(7), sizeof(A));

		fprintf(stderr, "maxSize=%ld, currSize=%ld\n", aCache.getMaxSize(), aCache.getCurrentSize());
		aCache.dumpUsage(stderr);
		aCache.dumpCache(stderr);
		fprintf(stderr, "Touching a3\n");
		aCache.touch("a3");
		fprintf(stderr, "Setting size to 2\n-------\n");
		aCache.setMaxSize(sizeof(A)*2);
		aCache.dumpUsage(stderr);
		fprintf(stderr, "maxSize=%ld, currSize=%ld\n", aCache.getMaxSize(), aCache.getCurrentSize());

		aCache.add("a3", new A(5), sizeof(A));

		fprintf(stderr, "Going out of scope\n");
	}

	exit(0);
}
