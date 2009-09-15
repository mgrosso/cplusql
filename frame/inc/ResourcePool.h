// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ResourcePool.h
// Author:    Matt Grosso
// Created:   Mon Dec  7 01:08:17 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: ResourcePool.h,v 1.1.1.1 1999/06/05 16:30:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H 1
#include <list>
#include "dbg.h"
#include "ptr.h"
#include "BaseException.h"
#include "Exclusion.h"

#define RPDBG( message ) FDBG( "ResourcePool", message, (unsigned long int)this );
#define RHDBG( message ) FDBG( "ResourceHandle", message, (unsigned long int)this );

template <class Resource > class ResourcePool ;

template < class Resource > 
class ResourceFactory { 
// ------------------------------------------------------------
// clients of ResourcePool will usually specialize this class. 
// ------------------------------------------------------------
public:
    Resource *makeResource(){ return new Resource();};
    void reclaimResource( Resource *r ){ delete r;};
    ~ResourceFactory(){};
};

template < class Resource >
class ResourceReturner {
public:
    static void do_delete( Resource *t ){
        (ResourcePool<Resource>::instance())->returnResource( t );
    }
};

class ShutdownException : public BaseException {
public:
    ShutdownException():
        BaseException("This ResourcePool has been shutdown."){};
};

class NotInitializedException : public BaseException {
public:
    NotInitializedException():
        BaseException("This ResourcePool has not been initialized."){};
};

template < class Resource >
class ResourcePool {
public:
    typedef ptr<Resource,ResourceReturner<Resource> >   Handle ;
    typedef ptr<ResourceFactory<Resource> >             Factory ;
    typedef list<Resource *>                            Pool ;
    friend class ResourceReturner<Resource> ;

public:
    static void init( Factory f, unsigned maxSize, unsigned startSize);
    static ResourcePool *instance();

    Handle      getResource();
    unsigned    getCurrentSize();
    unsigned    getCurrentUsed();
    unsigned    getMaxSize();

    void        setMaxSize(unsigned sz);
    void        setFactory(Factory f);
    ~ResourcePool();

private:
    ResourcePool( Factory f, unsigned maxSize, unsigned startSize);
    void returnResource( Resource * );

private:
    static ResourcePool<Resource> *instance_ ;
    Factory f_ ;
    Pool p_ ;
    Mutex m_ ;
    Condition c_ ;
    Condition done_ ;
    unsigned max_;
    unsigned out_;
    bool shutdown_ ;

private:
    //NOT IMPLEMENTED 
    ResourcePool();
    ResourcePool(const ResourcePool<Resource> &);
    ResourcePool<Resource> & operator= (const ResourcePool<Resource> &);
};

// ----------------------------------------------------------------------
// Static Declaration
// - reason why you can include this file only once per template type.
// ----------------------------------------------------------------------
template < class Resource >
ResourcePool<Resource> * ResourcePool<Resource>::instance_ ;

// ----------------------------------------------------------------------
// Implementation
// ----------------------------------------------------------------------
template < class Resource >
ResourcePool<Resource>::ResourcePool( 
    Factory f, unsigned maxSize, unsigned startSize
):f_(f),p_(),m_(),c_(),done_(),max_(maxSize),out_(0),shutdown_(false){
    if(startSize>max_)
        startSize=max_;
    for(unsigned i=0; i<startSize; ++i )
        p_.push_back( f_->makeResource() );
    RPDBG("ctor")
};

template < class Resource > void
ResourcePool<Resource>::init( 
    Factory f, unsigned maxSize, unsigned startSize ){
    if(!instance_)
        instance_=new ResourcePool<Resource>( f, maxSize, startSize );
};

template < class Resource > ResourcePool<Resource> *
ResourcePool<Resource>::instance( ){
    if(!instance_)
        throw NotInitializedException();
    return instance_;
};

template < class Resource > ResourcePool<Resource>::Handle
ResourcePool<Resource>::getResource(){
    RPDBG("getResource")
    MutexGuard lock(m_);
    while( out_ >= max_ && (!shutdown_))
        c_.wait(lock);
    if(shutdown_)
        throw ShutdownException();
    Resource *r;
    if( p_.size() >  0 ){
        r=p_.front();
        p_.pop_front();
    }
    else{
        r=f_->makeResource();
    }
    ++out_;
    return Handle(r);
};

template < class Resource > void
ResourcePool<Resource>::returnResource(Resource *r){
    RPDBG("returnResource")
    MutexGuard lock(m_);
    if( p_.size() >= max_ ){
        f_->reclaimResource( r );
    }
    else{
        p_.push_front(r);
    }
    if(--out_ < max_ && (!shutdown_))
        c_.signal();
    else if(shutdown_ && out_==0 )
        done_.signal();
};

template < class Resource > unsigned 
ResourcePool<Resource>::getCurrentSize(){
    return out_+p_.size();
};

template < class Resource > unsigned 
ResourcePool<Resource>::getCurrentUsed(){
    return out_;
};

template < class Resource > unsigned 
ResourcePool<Resource>::getMaxSize(){
    return max_;
};

template < class Resource > void
ResourcePool<Resource>::setMaxSize(unsigned newMax ){
    MutexGuard lock(m_);
    while(max_++<newMax && (!shutdown_))
        c_.signal();
};

template < class Resource > void
ResourcePool<Resource>::setFactory(Factory f){
    MutexGuard lock(m_);
    f_=f;
};

template < class Resource > 
ResourcePool<Resource>::~ResourcePool(){
    // set shutdown, then wait on shutdown condition.
    // when the last Handle reference goes out of scope, 
    // then this funciton will return.
    MutexGuard lock(m_);
    shutdown_=true;
    if(out_>0)
        done_.wait(lock);
    while(p_.size()){
        delete p_.front();
        p_.pop_front();
    }
};


#endif /* RESOURCEPOOL_H */

