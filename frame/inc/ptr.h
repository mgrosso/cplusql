// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ptr.h
// Author:    Matt Grosso
// Created:   Thu Oct 15 18:44:00 EDT 1998 on caliban.caliban.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  
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
// $Id: ptr.h,v 1.23 2004/06/23 16:55:08 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef PTR_H
#define PTR_H 1

#ifndef NEED_THREADS
#define NEED_THREADS
#endif /*  NEED_THREADS */
#include "frame_config.h"

#ifdef PTRDEBUG 
#undef DO_PTR_INLINE
#endif

#if ! defined DO_PTR_INLINE || DO_PTR_INLINE == 1
#define PTR_INLINE inline
#define DO_PTR_INLINE 1
#else
#define DO_PTR_INLINE 0
#define PTR_INLINE
#endif

//include of PtrSemaphore.h must happen after definition of PTR_INLINE
#include "PtrSemaphore.h"


#ifdef PTRDEBUG 
#include <unistd.h>
#include <stdio.h>
static void 
PTRDBG( const char *str, int line, long int j, unsigned int t, 
        unsigned int ref=0 );
#define PTRDBGS( str ) \
    PTRDBG( str, __LINE__, count(), (unsigned int )t_, (unsigned int)ref_ )
#else
#define PTRDBG( str, line, j, t, ref )
#define PTRDBGS( str ) 
#endif



template < class T >
class ptr_default_deleter 
{
public:
    PTR_INLINE static void do_delete ( T *t );
};

template < class T >
class ptr_array_deleter 
{
public:
    PTR_INLINE static void do_delete ( T *t );
};

template < class T >
class argv_deleter 
{
public:
    PTR_INLINE static void do_delete ( T *t );
};


template < class T, class Deleter = ptr_default_deleter<T> > 
class ptr 
{
public:
    enum { static_cast_tag, dynamic_cast_tag };
    PTR_INLINE ptr( );
    PTR_INLINE ptr( T *t );
    PTR_INLINE ptr( const ptr<T,Deleter> & );
    template < class U, class UDeleter > 
        PTR_INLINE ptr(const ptr<U, UDeleter > & rhs );
    template < class U, class UDeleter > 
        PTR_INLINE ptr( const ptr<U, UDeleter > & rhs, int tag );
    PTR_INLINE ~ptr();
    PTR_INLINE ptr<T,Deleter> & operator=( const ptr<T,Deleter> & );
    template < class U, class UDeleter> 
        PTR_INLINE ptr<T,Deleter> & operator=( const ptr<U,UDeleter> &rhsarg );
    PTR_INLINE ptr<T,Deleter> & operator=( T * );
    PTR_INLINE bool operator==( const ptr<T,Deleter> & ) const;
    PTR_INLINE bool operator!=( const ptr<T,Deleter> & ) const;
    PTR_INLINE const T & operator * () const;
    PTR_INLINE const T & operator [] ( int i ) const;
    PTR_INLINE const T * operator -> () const;
    PTR_INLINE const T * get() const;
    PTR_INLINE T & operator * () ;
    PTR_INLINE T & operator [] ( int i ) ;
    PTR_INLINE T * operator -> () ;
    PTR_INLINE T * get() ;
    PTR_INLINE T * release() ;
    PTR_INLINE int count();
protected:
    template < class U, class UDeleter > friend class ptr ;
    PtrSemaphore *ref_ ;
    T *t_ ;
};

// ----------------------------------------------------------------------
// implementation
// ----------------------------------------------------------------------

template < class T, class Deleter >
ptr<T,Deleter>::ptr( )
:t_(NULL)
{
    ref_=new PtrSemaphore();
    PTRDBGS( "ptr<T>::ptr() ctor" );
}

template < class T, class Deleter >
ptr<T,Deleter>::ptr( T *t )
:t_(t)
{
    ref_=new PtrSemaphore();
    PTRDBGS( "ptr<T>::ptr(T *t) ctor" );
}

template < class T, class Deleter >
ptr<T,Deleter>::ptr( const ptr<T,Deleter> &rhs )
:t_(rhs.t_)
{
    rhs.ref_->post();
    ref_=rhs.ref_;
    PTRDBGS( "ptr<T>::ptr( ptr<T> &rhs) copy ctor");
}

template < class T, class Deleter>
template < class U, class UDeleter > 
ptr<T,Deleter>::ptr(const ptr<U, UDeleter > & rhs )
:t_(dynamic_cast<T *>(rhs.t_))
{
    rhs.ref_->post();
    ref_=rhs.ref_;
}

template < class T, class Deleter>
template < class U, class UDeleter > 
ptr<T,Deleter>::ptr( const ptr<U, UDeleter > & rhs, int tag )
:t_(static_cast<T *>(rhs.t_))
{
    rhs.ref_->post();
    ref_=rhs.ref_;
}

template < class T, class Deleter >
int 
ptr<T,Deleter>::count()
{
    int gb;
    ref_->getvalue(&gb);
    return gb ;
}

template < class T, class Deleter >
ptr<T,Deleter>::~ptr()
{
    ref_->wait();
    if(0<count()){
        PTRDBGS( "ptr<T>::~ptr() returning");
        return;
    }
    PTRDBGS( "ptr<T>::~ptr() destroying");
    if(t_!=NULL){
        Deleter::do_delete( t_);
        t_=NULL;
    }
    delete ref_ ;
	ref_ = (PtrSemaphore *) 0;
}

template < class T, class Deleter >
T *
ptr<T,Deleter>::release()
{
    if(1 != count()){
        PTRDBGS( "ptr<T>::release() count() not 1!");
        return NULL;
    }
    PTRDBGS( "ptr<T>::release() releasing");
    T *gb = t_;
    t_ = NULL ;
    return gb ;
}

template < class T, class Deleter >
template < class U, class UDeleter> 
ptr<T,Deleter> & 
ptr<T,Deleter>::operator=( const ptr<U,UDeleter> &rhsarg )
{
    ptr<U,UDeleter> &rhs=const_cast< ptr <U, UDeleter >& > ( rhsarg );
    PTRDBGS( "ptr<T>::op=() this");
    PTRDBG( "ptr<T>::op=() rhs", __LINE__,rhs.count(), (unsigned int ) rhs.t_, t_ );
    if( rhs.ref_==ref_ )
    {
        return *this ;//self assignment
    }
    rhs.ref_->post();
    ref_->wait();
    if(1>count()){
        PTRDBGS( "ptr<T>::op=() destroying former this");
        if(t_!=NULL){
            Deleter::do_delete( t_);
            t_=NULL;
        }
        delete ref_ ;
    }
    t_=rhs.t_;
    ref_=rhs.ref_;
    return *this;
}

template < class T, class Deleter >
ptr<T,Deleter>& 
ptr<T,Deleter>::operator=( const ptr<T,Deleter> &rhsarg )
{
    ptr<T,Deleter> &rhs=const_cast< ptr <T, Deleter >& > ( rhsarg );
    PTRDBGS( "ptr<T>::op=() this");
    PTRDBG( "ptr<T>::op=() rhs", __LINE__,rhs.count(), (unsigned int ) rhs.t_,rhs.ref_ );
    if( rhs.ref_==ref_ )
        return *this ;//self assignment
    rhs.ref_->post();
    ref_->wait();
    if(1>count()){
        PTRDBGS( "ptr<T>::op=() destroying former this");
        if(t_!=NULL){
            Deleter::do_delete( t_);
            t_=NULL;
        }
        delete ref_ ;
    }
    t_=rhs.t_;
    ref_=rhs.ref_;
    return *this;
}

template < class T, class Deleter >
ptr<T,Deleter>& 
ptr<T,Deleter>::operator=( T *t )
{
    PTRDBGS( "ptr<T>::op=(T *t) this");
    if( t==t_ )
        return *this ;//self assignment
    ref_->wait();
    if(1>count()){
        PTRDBGS( "ptr<T>::op=(T*) destroying former this");
        if(t_!=NULL){
            Deleter::do_delete( t_);
            t_=NULL;
        }
        delete ref_ ;
    }
    ref_ = new PtrSemaphore();
    t_ = t ;
    PTRDBGS( "ptr<T>::ptr() op=(T*t)" );
    return *this;
}

template < class T, class Deleter >
bool 
ptr<T,Deleter>::operator==( const ptr<T,Deleter> &rhs ) const 
{
    return ref_==rhs.ref_;
}

template < class T, class Deleter >
bool 
ptr<T,Deleter>::operator!=( const ptr<T,Deleter> &rhs ) const 
{
    return ref_!=rhs.ref_;
}

template < class T, class Deleter >
const T & 
ptr<T,Deleter>::operator *() const 
{
    return *(get());
}

template < class T, class Deleter >
const T & 
ptr<T,Deleter>::operator []( int i ) const 
{
    return (get())[i];
}

template < class T, class Deleter >
const T * 
ptr<T,Deleter>::operator ->() const 
{ 
    return get();
}

template < class T, class Deleter >
const T * 
ptr<T,Deleter>::get() const 
{
    return t_;
}

template < class T, class Deleter >
T & 
ptr<T,Deleter>::operator *() 
{
    return *(get());
}

template < class T, class Deleter >
T & 
ptr<T,Deleter>::operator []( int i ) 
{
    return (get())[i];
}

template < class T, class Deleter >
T * 
ptr<T,Deleter>::operator ->() 
{ 
    return get();
}

template < class T, class Deleter >
T * 
ptr<T,Deleter>::get() 
{
    return t_;
}

template < class T >
void 
ptr_default_deleter< T >::do_delete ( T *t )
{
    delete t;
}

template < class T >
void ptr_array_deleter< T >::do_delete ( T *t )
{
    delete [] t;
}

template < class T >
void argv_deleter< T >::do_delete ( T *t )
{
    T * i=t;
    for( ; i && *i ; ++i )
    {
        delete [] *i;
    }
    delete [] t;
}

#ifdef PTRDEBUG 
void 
PTRDBG( const char *str, int line, long int j, unsigned int t, unsigned int ref=0 )
{
    fprintf(stderr,"ptr[%x][%li][%s][%i] [%s][%li][%x][%x]\n", 
        GET_ULONG_THR_SELF(), getpid(), 
        __FILE__, line, str, j, t, ref 
    );
}
#endif

#endif /* PTR_H */
