// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PtrSemaphore.h
// Author:    mgrosso Matthew E Grosso
// Created:   Sun Dec 14 13:58:58 EST 2003 on erasmus.erasmus.org
// Project:   
// Purpose:   
//
// ptr use the value of the semaphore as a reference count, so it starts 
// off at 1, and only reaches 0 once, and never needs to block on a lock
// since the semaphore is really only being used for atomic increment and 
// decrement. Knowing this, we can use a stripped down version that consumes
// less memory and less code than a normal semaphore and is much faster, 
// since it only locks the bus for one instruction. This implementation is
// for x86, but should be trivially portable to any platform that supports
// an atomic exchange/add function.
// 
// First motivated by WIN32 + g++, but should work on any g++ x86 platform.
// To work for ptr.h a semaphore implementation must support getvalue()
// and WIN32 does not provide a reasonable way of doing that.
//
// note that wait() will not actually wait unless we also add 
// something like a pthread_cond_t. for now, it does trywait() or returns
// ENOSYS
//
// To work as a general purpose semaphore, we would need to add a spinlock
// and a way for threads to block without polling. Most platforms allready
// have semaphores though, so why would you do that?
//
// Copyright (c) 2003 Matthew E Grosso. 
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
// $Id: PtrSemaphore.h,v 1.6 2004/05/27 21:57:49 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PTRSEMAPHORE_H
#define PTRSEMAPHORE_H 1

#include "frame_config.h"

#if PTR_USES_ASM_SEMAPHORE == 1
class PtrSemaphore
{
private:
    volatile long v_;
public:
    PTR_INLINE PtrSemaphore();
    PTR_INLINE PtrSemaphore(int value);
    PTR_INLINE ~PtrSemaphore();
    PTR_INLINE volatile int wait();
    PTR_INLINE volatile int trywait();
    PTR_INLINE volatile int post();
    PTR_INLINE volatile void getvalue(  volatile int *v );
private:
    PTR_INLINE volatile long lock_xadd(  volatile long i );
};
#if defined DO_PTR_INLINE 
#include "PtrSemaphore.i"
#endif /* DO_PTR_INLINE  */

#else
#include "Semaphore.h"
typedef Semaphore PtrSemaphore ;
#endif/* PTR_USES_ASM_SEMAPHORE */



/**
*/

class Counter
{
public:
	inline explicit Counter(PtrSemaphore &s);
	inline explicit Counter(const Counter &c);
	inline ~Counter();
    
	inline int getCount();

private:
	// Private to disallow.
	// 
	explicit Counter();
	Counter &operator=(const Counter &);
    
	PtrSemaphore &s_;
};

Counter::Counter(PtrSemaphore &s) : s_(s)
{
	s_.post();
};


Counter::Counter(const Counter &c) : s_(c.s_)
{
	s_.post();
};


Counter::~Counter()
{
	s_.trywait();
};


int
Counter::getCount()
{
	int retVal = 0;

	s_.getvalue(&retVal);

	return retVal;
};


#endif /* PTRSEMAPHORE_H */
