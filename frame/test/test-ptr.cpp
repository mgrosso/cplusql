#ident  "file_id $Id: test-ptr.cpp,v 1.5 2004/01/10 21:19:07 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      main.cpp
// Author:    Matt Grosso
// Created:   Wed Oct 14 20:42:59 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  
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
// $Id: test-ptr.cpp,v 1.5 2004/01/10 21:19:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include <stdio.h>
#include <vector>
#include <map>
#include <assert.h>
#define DEBUG 1
#include "ptr.h"
#include "ptr_strdup.h"


class C {
	public:
		C():id_(0){ 
			fprintf(stderr,"C()%i class cntr=%i\n",id_, ++cntr_ );
		};
		C( int id):id_(id){ 
			fprintf(stderr,"C(int)%i class cntr=%i\n",id_, ++cntr_ );
		};
		virtual ~C(){ 
			fprintf(stderr,"~C()%i class cntr=%i\n",id_, --cntr_ );
            if( cntr_ < 0 )
                fprintf(stderr,"ALERT ~C()");
		};
		C(const C &rhs){
			id_=rhs.id_; 
			fprintf(stderr,"C(const C &)%i class cntr=%i\n",id_, ++cntr_ );
		};
		C & operator=(const C &rhs){
			fprintf(stderr,"C & operator=(const C &)%i rhs%i\n",id_,rhs.id_);
			id_=rhs.id_;
			return *this;
		};
        static int getCounter(){ 
            return cntr_;
        };
	protected:
		int id_;
        static int cntr_;
};
int C::cntr_=0;

class D : public C { 
public:
    D( int id):C(id+52),x(id){ 
        fprintf(stderr,"D()id=%i,x=%i\n",id_,x);
    };
    virtual ~D(){
        fprintf(stderr,"~D()%i,%i\n",id_,x);
    };
    int x;
};

void ptrstltest()
{
    map< int, vector< ptr< C > > > mvpc;
    for( int i=0; i< 100; ++i )
    {
        vector< ptr< C > > vpc;
        fprintf(stderr,"ptrstltest()%i\n",i);
        for( int j=0; j< 100; ++j )
        {
            ptr<C> cn( new C(j));
            vpc.push_back(cn);
        }
        mvpc[i]=vpc;
    }
};

typedef long long mykey_t;
void ptrstltest2()
{
    map< mykey_t , vector< ptr< C > > > mvpc;
    for( mykey_t i=0; i< 100; ++i )
    {
        vector< ptr< C > > vpc;
        for( mykey_t j=0; j< 100; ++j )
        {
            ptr<C> cn( new C(j));
            vpc.push_back(cn);
        }
        mvpc[i]=vpc;
    }
};

int main( int argc, char **argv ){
	fprintf(stderr,"welcome to frame\n");
    {
        const char *deadbeef = "deadbeef";
        char_ptr myc(ptr_strdup(deadbeef));
    }
	{
		ptr<int> i( new int);
		ptr<int> j( new int);
		*i=69;
		*j=70;
		fprintf(stderr,"i[%i] j[%i]\n", *i, *j );
        assert( *i==69 );
        assert( *j==70 );
		int tmp=*i;
		*(i.get())=*(j.get());
		*j=tmp;
		fprintf(stderr,"i[%i] j[%i]\n", *i, *j );
        assert( *i==70 );
        assert( *j==69 );
		i=j;
		fprintf(stderr,"i[%i] j[%i]\n", *i, *j );
        assert( *i==69 );
        assert( *j==69 );
	}

    assert( 0==C::getCounter());
    try{
		ptr<C> c0( new C(1));
	}catch(... ){
		fprintf(stderr,"exception\n");
        exit(1);
	}
    assert( 0==C::getCounter());
	try{
		ptr<C> c1( new C(1));
		ptr<C> c2( new D(2));
		c1=c2;
		ptr<C> c3( new C(3));
		*c2=*c3;
		ptr<C> c4( new C(4));
	}catch(... ){
		fprintf(stderr,"exception\n");
        exit(1);
	}
    assert( 0==C::getCounter());

    try{
        ptr<char,ptr_array_deleter<char> > foo( new char[1024] );
        strcpy( foo.get(), "hello from array_ptr!!\n" );
        fprintf(stderr, "%s", foo.get() );
        ptr<char,ptr_array_deleter<char> > bar(foo);
        ptr<char,ptr_array_deleter<char> > another=foo;
        bar=foo;
        foo=bar;
    }catch(...){
		fprintf(stderr,"exception\n");
        exit(1);
    }
    try{
        fprintf(stderr,"beginning vector ptr test #1\n");
        ptrstltest();
        assert( 0==C::getCounter());
        fprintf(stderr,"beginning vector ptr test #2\n");
        ptrstltest2();
        assert( 0==C::getCounter());
        fprintf(stderr,"done with vector ptr test\n");
    }catch(...){
		fprintf(stderr,"exception\n");
        exit(1);
    }
    fprintf(stderr,"beginning empty ptr test\n");
    try{
        ptr<int> fu();
        ptr<char,ptr_array_deleter<char> > bar();
        char_ptr fubar;
        char_ptr test2(ptr_strdup("deadbeef"));
    }catch(...){
		fprintf(stderr,"exception\n");
        exit(1);
    }
    {
        const char *deadbeef = "deadbeef";
        char_ptr myc(ptr_strdup(deadbeef));
    }
    fprintf(stderr,"done empty ptr test\n");
    assert( 0==C::getCounter());
	fprintf(stderr,"bye\n");
	return 0;
};
