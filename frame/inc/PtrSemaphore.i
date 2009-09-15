// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PtrSemaphore.i
// Author:    mgrosso Matthew E Grosso
// Created:   Fri Dec 26 17:01:16 EST 2003 on erasmus.erasmus.org
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 Matthew E Grosso. 
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
// $Id: PtrSemaphore.i,v 1.3 2004/03/24 21:29:35 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <errno.h>

PtrSemaphore::PtrSemaphore()
:v_(1)
{ 
    ;//noop
};

PtrSemaphore::PtrSemaphore(int value)
:v_( static_cast< long > ( value))
{ 
    ;//noop
};

PtrSemaphore::~PtrSemaphore()
{ 
    ;//noop
};

volatile int 
PtrSemaphore::wait()
{ 
    if( 0!= trywait())
    {
        //TODO add something like pthread_cond_wait() here to get a complete
        //semaphore. not necessary for use by ptr.h only.
        errno = ENOSYS ;
        return -1 ;
    }
    return 0;
};

volatile int 
PtrSemaphore::trywait()
{ 
    volatile long i = lock_xadd(-1);
    if( i-1 < 0 )
    {
        i=lock_xadd(1);//lock_xadd must assign to something volatile...
        errno = EAGAIN ;
        return -1 ;
    }
    return 0;
};

volatile int 
PtrSemaphore::post()
{ 
    volatile long dummy ;
    dummy = lock_xadd(1);
    return 0;
};

volatile void 
PtrSemaphore::getvalue( volatile int *i )
{
    *i=static_cast<int>(lock_xadd(0));
};

volatile long 
PtrSemaphore::lock_xadd( volatile long i )
{
#ifdef single_threaded_non_x86
	long pre = v_;
	v_ += i;

   // fprintf(stderr, "adding %ld to %ld to give %ld, returning %ld\n", i, pre, v_, pre);

	return pre;
#else
    volatile long j=i;
    volatile unsigned long p = (unsigned long )&v_ ;

    asm( "lock ; xadd %0, (%1)" : "+r"(j) : "r"(p) );

    return j;
#endif
};
