#ident "file_id $Id: ClusterByteSource.cpp,v 1.9 2005/10/17 11:18:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ClusterByteSource.cpp
// Author:    mgrosso 
// Created:   Sun Apr 27 02:03:34 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ClusterByteSource.cpp,v 1.9 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "cplusql.h"
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#include "ClusterByteSource.h"
#include "fwrap.h"
#include "BaseException.h"
#include "serviceC.h"
#include "AppLog.h"
#include "SharedServiceMain.h"
#include "hashFuncs.h"
#include "CRC48HashAlg.h"
#include <map>

#include <unistd.h>

typedef ptr<Service::ServiceRef> ServiceRefPtr;
typedef clusterio::ReadWriteService_ptr ReadWriteServicePtr;
typedef map<const char_ptr, ReadWriteServicePtr, char_ptr_less_than > ReadWriteServiceMapType;

class ClusterByteSourcePriv
{
private:
    friend class ClusterByteSource ;

    ClusterByteSourcePriv( clusterio::NameSpaceService_ptr nss, clusterio::open_info fd );

    clusterio::NameSpaceService_ptr nss_;
    const clusterio::open_info       fd_;
    ReadWriteServiceMapType readWriteServiceMap_;

    clusterio::length_t currentStartOffset_;

    bool done_;
};

ClusterByteSourcePriv::ClusterByteSourcePriv( clusterio::NameSpaceService_ptr nss, clusterio::open_info fd)
: nss_(nss), fd_(fd), currentStartOffset_(0), done_(false)
{
    ;//
}

ClusterByteSource::ClusterByteSource( clusterio::NameSpaceService_ptr nss, const char *filename )
{
    PANIC("not implemented, yet");
    //d_=new ClusterByteSourcePriv( fwrap::open_read( filename), true );
}

ClusterByteSource::ClusterByteSource( 
    clusterio::NameSpaceService_ptr nss, clusterio::open_info fd )
{
    d_=new ClusterByteSourcePriv( nss, fd );
}

ClusterByteSource::~ClusterByteSource()
{
    delete d_ ;
}

bool 
ClusterByteSource::next( char *buffer, size_t &bytes_read, size_t bufsize )
{
    if(d_->done_)
    {
        return false;
    }
    char_ptr desc ;
    //TODO we should pass io exceptions to the nss to find out what to do.
    //strncpy( buffer, "0xdeadbeef", bufsize-1 );
    try {
        if(!bufsize)
        {
            APPLOG_WARN(
                "zero length reads return true without modifying buffer.");
            bytes_read = 0;
            return true;
        }
        if(!buffer)
        {
            PANIC("ClusterByteSource::next(), null buffer");
        }
        if(d_->fd_.servants.length() < 1)
        {
            PANIC("ClusterByteSource::next(), no servants in list");
        }

        // pick this randomly or by algorithm eventually. now take the first one
        int servantIndex=0;
        Service::ServiceRef ref = d_->fd_.servants[servantIndex];

        ReadWriteServicePtr prws = NULL;
        static const int max_attempt = 2;
        const char* objectString = ref.ior;

        char_ptr pUrl = ptr_strdup(ref.url);
        if(d_->readWriteServiceMap_.find(pUrl) != d_->readWriteServiceMap_.end())
        {
            prws = d_->readWriteServiceMap_[pUrl];
        }
        else
        {
            APPLOG_DBG("resolve %s", objectString);

            CORBA::Object_ptr obj = 
                SharedServiceMain::instance()->resolve(objectString);
            if(!obj)
            {
                PANICV("resolve(%s) returned NULL", objectString );
            }
            APPLOG_DBG("narrow %s", objectString);
            prws = clusterio::ReadWriteService::_narrow( obj );
            if(!prws)
            {
                PANICV("_narrow() returned NULL" );
            }
            d_->readWriteServiceMap_[pUrl] = prws;
        }

        clusterio::file_data_t data(bufsize, bufsize, reinterpret_cast<CORBA::Octet*>(buffer), 0);
        clusterio::file_data_t *datap = &data ;

        clusterio::file_data_t_out data_out(datap);
        
        //APPLOG_DBG("reading ... %llu", d_->fd_ );
        bytes_read = prws->read( d_->fd_,
                                    d_->currentStartOffset_,
                                    bufsize,
                                    data_out);
        
        if( bytes_read < bufsize )
        {
            d_->done_ = true ;
        }
        if (bytes_read == 0)
        {
            return false;
        }
        d_->currentStartOffset_ +=  bytes_read;
        //
        // TODO: sadly, the debugging below shows that we are not getting zero copy
        // semantics at this point.  For now, we must memcpy.
        //
        // possibly the file_data_t typedef prevents tao idlgen from emitting
        // the specialized template code.  Another possibility is that this
        // behavior requires defining TAO_NO_COPY_OCTET_SEQUENCES and using the
        // ACE_Message_Block typedefs.
        //
        //

        //unclear why why memcpy is hosing memory, but it is, so we do a byte for 
        //byte copy instead. ... and no, the memory regions do not overlap.
        //
        //memcpy( buffer, data_out.ptr(), bytes_read );
        for( size_t i=0; i<bytes_read; ++i )
        {
            buffer[i] = data_out[i];
        }

        //char_ptr dbgout = ptr_strdup_len( buffer, bytes_read );
        //APPLOG_DBG( "just read %u bytes into %llx %llx %llx: %s", 
        //    bytes_read, 
        //    reinterpret_cast<unsigned long long>(buffer), 
        //    reinterpret_cast<unsigned long long>(& data[0]), 
        //    reinterpret_cast<unsigned long long>( data_out.ptr()),
        //    dbgout.get());
        //for( size_t i=0; i<bytes_read; ++i )
       // {
       //     APPLOG_DBG( "char %u/%u = %x, %x, %x", 
        //        i,
        //        bytes_read,
        //        static_cast<int>(buffer[i]),
        //        static_cast<int>(data[i]),
        //        static_cast<int>(data_out[i])
        //    );
        //}
        //APPLOG_DBG( "just read %u bytes", bytes_read );

        return true;
    }
    catch( clusterio::ClusterIOException &cioe )
    { 
        //
        //TODO if new() throws here, we'll abort. this needs a 
        //purely stack based implementation.
        //
        desc = "{" ;
        unsigned int sz = cioe.exceptions.length();
        for( unsigned int i=0; i<sz; ++i )
        {
            desc = desc + ptr_strdup_long( i );
            desc = desc + "={";
            desc = desc + cioe.exceptions[i].serviceRef.url.in() ;
            desc = desc + "," ;
            desc = desc + ptr_strdup_long( cioe.exceptions[i].type );
            desc = desc + "," ;
            desc = desc + ptr_strdup_long( cioe.exceptions[i].errno_short );
            desc = desc + "," ;
            desc = desc + cioe.exceptions[i].what_string.in();
            desc = desc + "}" ;
        }
        desc = desc + "}" ;
        PANICV( "ClusterIOExcepton %s", desc.get());
    }
    catch( Service::ServiceException &e )
    {
        //
        //TODO if new() throws here, we'll abort. this needs a 
        //purely stack based implementation.
        //
#ifdef HAVE_STRERROR_R
        char strerrbuf[1024];
        memset( strerrbuf, '\0', 1024 );
        strerror_r( e.errno_short, strerrbuf, 1023 );
#else  /* HAVE_STRERROR_R */
        desc = desc + ptr_strdup_long( e.errno_short );
#endif  /* HAVE_STRERROR_R */
        desc = desc + ", ";
        desc = desc + e.what_string.in();
        desc = desc + " }";
        PANICV("ServiceException{ %s", desc.get());
    }
    //not reached.
    return false;
}



#endif
#endif
