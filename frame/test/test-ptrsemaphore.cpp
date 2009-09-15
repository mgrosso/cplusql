#ident  "file_id $Id: test-ptrsemaphore.cpp,v 1.2 2004/01/10 21:19:07 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-ptrsemaphore.cpp
// Author:    mgrosso Matthew E Grosso
// Created:   Fri Dec 26 17:21:06 EST 2003 on erasmus.erasmus.org
// Project:   
// Purpose:   
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
// $Id: test-ptrsemaphore.cpp,v 1.2 2004/01/10 21:19:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define DO_PTR_INLINE 1
#define PTR_INLINE inline
#define PTR_USES_ASM_SEMAPHORE 1
#include "PtrSemaphore.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int dbggv( PtrSemaphore &p )
{
    volatile int i;
    p.getvalue(&i);
    return i;
}

int main( int argc, char **argv )
{
    fprintf(stderr, "hello, today we're testing PtrSemaphore\n" );

    fprintf(stderr, "testing " );
    {
        PtrSemaphore p;
        PtrSemaphore p0(0);
        PtrSemaphore p1(1);
    }
    fprintf(stderr, "." );

    {
        PtrSemaphore p;
        assert( 1==dbggv(p));
        fprintf(stderr, "." );
        PtrSemaphore p0(0);
        assert( 0==dbggv(p0));
        fprintf(stderr, "." );
        PtrSemaphore p1(1);
        assert( 1==dbggv(p1));
        fprintf(stderr, "." );
    }
    fprintf(stderr, "." );

    {
        PtrSemaphore p(0);
        assert( 0==dbggv(p));
        fprintf(stderr, "." );
        p.post();
        assert( 1==dbggv(p));
        fprintf(stderr, "." );
        p.post();
        assert( 2==dbggv(p));
        fprintf(stderr, "." );
        p.trywait();
        assert( 1==dbggv(p));
        fprintf(stderr, "." );
        p.trywait();
        assert( 0==dbggv(p));
        fprintf(stderr, "." );
        p.post();
        p.post();
        p.wait();
        assert( 1==dbggv(p));
        fprintf(stderr, "." );
        p.wait();
        assert( 0==dbggv(p));
        fprintf(stderr, "." );
    }
    fprintf(stderr, "." );

    {
        static const int N=1000000 ;
        PtrSemaphore p(0);
        assert( 0==dbggv(p));
        fprintf(stderr, "." );
        for( int i=1; i <= N; ++i )
        {
            p.post();
            assert( i==dbggv(p));
        }
        fprintf(stderr, "." );
        for( int i=N; i >0 ; --i )
        {
            p.wait();
            assert( i-1 == dbggv(p));
        }
        fprintf(stderr, "." );
        for( int i=1; i <= N; ++i )
        {
            p.post();
            assert( i==dbggv(p));
        }
        fprintf(stderr, "." );
        for( int i=N; i >0 ; --i )
        {
            p.wait();
            assert( i-1 == dbggv(p));
        }
        fprintf(stderr, "." );
    }
    fprintf(stderr, "." );

    {
        PtrSemaphore p(0);
        assert( -1 == p.trywait() && EAGAIN == errno );
        fprintf(stderr, "." );
        assert( 0 == dbggv(p));
        fprintf(stderr, "." );
        assert( -1 == p.wait() && ENOSYS == errno );
        fprintf(stderr, "." );
        assert( 0 == dbggv(p));
        fprintf(stderr, "." );
    }
    fprintf(stderr, " ok\n" );
    exit(0);
};



