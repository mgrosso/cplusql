#ident "file_id $Id: Semaphore.cpp,v 1.6 2004/01/10 21:19:07 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Semaphore.h
// Author:    mgrosso 
// Created:   Thu Mar  1 16:38:15 EST 2001 on dev1.int.primaryknowledge.com
// Project:   
// Purpose:   os independent semaphore class. should work for linux and solaris
// 
// Copyright (c) 2001 Matt Grosso.
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
// $Id: Semaphore.cpp,v 1.6 2004/01/10 21:19:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#define NEED_THREADS
#include "frame_config.h"
#include "Semaphore.h"

#if FRAME_USES_PTHREAD_SEMAPHORE == 1
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// linux implementation
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
extern "C" {
#include "semaphore.h"
#include "pthread.h"
};

Semaphore::Semaphore()
{ 
    s_=new sem_t;
    sem_init( (sem_t *)s_,0,1);
};
Semaphore::Semaphore(int value)
{ 
    sem_init( (sem_t *)s_,0,  value > SEM_VALUE_MAX ? SEM_VALUE_MAX : value );
};
Semaphore::~Semaphore()
{ 
    sem_destroy( (sem_t *)s_);
    delete (sem_t *)s_;
};
int 
Semaphore::wait()
{ 
    return sem_wait( (sem_t *)s_ );
};
int 
Semaphore::trywait()
{ 
    return sem_trywait( (sem_t *)s_ );
};
int 
Semaphore::post()
{ 
    return sem_post( (sem_t *)s_ );
};
void 
Semaphore::getvalue( int *v )
{ 
    sem_getvalue( (sem_t *)s_, v );
};

#elif defined FRAME_USES_BROKEN_ASM_SEMAPHORE
#error "this is not the droid you are looking for."
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// motivated by WIN32 + g++, but should work on any g++ x86 platform.
//
// note that wait() will not actually wait unless we also add 
// something like a pthread_cond_t. for now, it does trywait().
//
// To work for ptr.h a semaphore implementation must support getvalue()
// and WIN32 does not provide a reasonable way of doing that.
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include <errno.h>

class SemaphorePriv {
    friend class Semaphore;
    private:
    SemaphorePriv( long v )
    :v_(v)
    {
        ;//noop
    };
    long v_;
    //pthread_cond_t ....
};
Semaphore::Semaphore()
:s_( reinterpret_cast< void *> ( new SemaphorePriv(1)))
{ 
    ;//noop
};
Semaphore::Semaphore(int value)
:s_( reinterpret_cast< void *> ( new SemaphorePriv(value)))
{ 
    ;//noop
};
Semaphore::~Semaphore()
{ 
    delete reinterpret_cast< SemaphorePriv * > ( s_ );
};
int 
Semaphore::wait()
{ 
    if( 0!= trywait())
    {
        //TODO add something like pthread_cond_wait() here.
        errno = ENOSYS ;
        return -1 ;
    }
    return 0;
};
int 
Semaphore::trywait()
{ 
    unsigned long p = reinterpret_cast< unsigned long > ( 
        &(reinterpret_cast< SemaphorePriv * > ( s_ ))->v_ );
    long i = -1;
    asm( "lock ; xadd %0, (%1)" : "+r"(i) : "r"(p) );
    if( i-1 < 0 )
    {
        i=1;
        asm( "lock ; xadd %0, (%1)" : "+r"(i) : "r"(p) );
        return EAGAIN;
    }
    return 0;
};
int 
Semaphore::post()
{ 
    unsigned long p = reinterpret_cast< unsigned long > ( 
        &(reinterpret_cast< SemaphorePriv * > ( s_ ))->v_ );
    long i = 1;
    asm( "lock ; xadd %0, (%1)" : "+r"(i) : "r"(p) );
    return 0;
};
void 
Semaphore::getvalue( int *v )
{ 
    unsigned long p = reinterpret_cast< unsigned long > ( 
        &(reinterpret_cast< SemaphorePriv * > ( s_ ))->v_ );
    long i = 0;
    asm( "lock ; xadd %0, (%1)" : "+r"(i) : "r"(p) );
    *v=static_cast<int>(i);
};

#elif FRAME_USES_SOLARIS_SEMAPHORE == 1

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// solaris implementation
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

extern "C" {
#include "synch.h"
#include "errno.h"
};

Semaphore::Semaphore()
{ 
    s_= (void *)new sema_t;
    sema_init( (sema_t *)s_,1,USYNC_THREAD,NULL);
};
Semaphore::Semaphore(int value)
{ 
    s_= (void *)new sema_t;
    sema_init( (sema_t *)s_, value > SEM_VALUE_MAX ? SEM_VALUE_MAX : value , USYNC_THREAD, NULL ); 
};
Semaphore::~Semaphore()
{ 
    sema_destroy( (sema_t *)s_);
    delete (sema_t *)s_;
};
int 
Semaphore::wait()
{ 
    return sema_wait( (sema_t *)s_ ); 
};
int 
Semaphore::trywait()
{ 
    return sema_trywait( (sema_t *)s_ );
};
int 
Semaphore::post()
{ 
    return sema_post( (sema_t *)s_ );
};
void 
Semaphore::getvalue( int *v )
{ 
    //hack, we peek at internals
    *v = ((sema_t *)s_)->count ;
};

#elif FRAME_USES_ACE_SEMAPHORE == 1

Semaphore::Semaphore()
{ 
    s_= (void *)new ACE_sema_t();
    ACE_OS::sema_init( (ACE_sema_t *)s_, 1 );
};
Semaphore::Semaphore(int value)
{ 
    s_= (void *)new ACE_sema_t();
    ACE_OS::sema_init( (ACE_sema_t *)s_, value > SEM_VALUE_MAX ? SEM_VALUE_MAX : value );
};
Semaphore::~Semaphore()
{ 
    ACE_OS::sema_destroy( (ACE_sema_t *)s_ );
    delete (ACE_sema_t *)s_;
};
int 
Semaphore::wait()
{ 
    return ACE_OS::sema_wait( (ACE_sema_t *)s_ ); 
};
int 
Semaphore::trywait()
{ 
    return ACE_OS::sema_trywait( (ACE_sema_t *)s_ );
};
int 
Semaphore::post()
{ 
    return ACE_OS::sema_post( (ACE_sema_t *)s_ );
};

//    ACE_OS:: semaphores dont do getvalue(). this will be caught at link time.
//void 
//Semaphore::getvalue( int *v )
//{ 
//#error "ACE_OS:: semaphores wont do getvalue(). try --enable-asm-ptr-semaphore."
//};

#else
#error you must define one of FRAME_USES_foo_SEMAPHORE to 1 where foo is either PTHREAD or SOLARIS
#endif /* FRAME_USES_PTHREAD_SEMAPHORE */
