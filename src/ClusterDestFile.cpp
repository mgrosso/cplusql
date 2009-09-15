#ident "file_id $Id: ClusterDestFile.cpp,v 1.11 2005/10/17 11:18:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ClusterDestFile.cpp
// Author:    
// Created:   
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: ClusterDestFile.cpp,v 1.11 2005/10/17 11:18:58 mgrosso Exp $ 
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "cplusql_config.h"
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1

#include "BaseException.h"
#include "NameSpaceServiceImpl.h"
#include "SharedServiceMain.h"
#include "ClusterDestFile.h"
#include "CRC48HashAlg.h"

//
// helper class for ClusterDestFile
// -- obfuscates mutable interface/implementation so as to require less
//    re-compilation upon vicissitudes
//
class ClusterDestFilePriv
{
    private:
    friend class ClusterDestFile;

    ClusterDestFilePriv(
        const clusterio::NameSpaceService_ptr nss,
        const clusterio::writer_id writer_id,
        const char *filename
    );

    void selectReadWriteService();
    bool rollback();
    bool append( clusterio::blockseq & blox );
    bool gotFailure( clusterio::IOExceptionStructList * cioe_exceptions );

    clusterio::open_info * _fd;
    clusterio::NameSpaceService_ptr _nss;
    clusterio::writer_id _writer_id;
    clusterio::ReadWriteService_ptr _RWS_ptr;
    CORBA::ULong _next_sequence_num;
};

//
// constructor -- first task is to get open_info from nss, and subsequently
//     select the first rws in the servants list for use
//
ClusterDestFilePriv::ClusterDestFilePriv( 
    const clusterio::NameSpaceService_ptr nss,
    const clusterio::writer_id writer_id,
    const char *filename
):_nss(nss),_writer_id(writer_id)
{
    // get an open_info "filehandle" from the namespace service 
    APPLOG_DBG( "ClusterDestFilePriv::ClusterDestFilePriv open %s", filename );
    _fd = nss->open( filename, 1, clusterio::o_wronly + clusterio::o_creat,
                     SharedServiceMain::instance()->myHostname());
                     // raises( Service::ServiceException );

    // update ReadWriteService pointer (first servant in open_info struct)
    selectReadWriteService();
}

bool
ClusterDestFilePriv::rollback()
{
    // failure details
    clusterio::IOExceptionStructList * cioe_exceptions = NULL; 

    for( short attempts = 0; attempts < 3; attempts++ )
    {
        try{
            APPLOG_DBG( "ClusterDestFilePriv::rollback _next_sequence_num=[%d]",
                        _next_sequence_num );
             
            if( _RWS_ptr == NULL ) { PANIC( "_RWS_ptr is NULL" ); }
            _RWS_ptr->undo_appends( *_fd, _writer_id, _next_sequence_num );
               // raises ClusterIOException, Service::ServiceException
        }
        catch( clusterio::ClusterIOException & cie )
        {
            // an error occured that requres guidance from NameSpaceService
            cioe_exceptions = &cie.exceptions;
        }

        // if no failure, all done
        if( ! ClusterDestFilePriv::gotFailure( cioe_exceptions))
        { return true; }
    }
    return false;
}

bool
ClusterDestFilePriv::append( clusterio::blockseq & blox )
{
   // failure details
    clusterio::IOExceptionStructList * cioe_exceptions = NULL;

    for( short attempts = 0; attempts < 3; attempts++ )
    {
        try{
            APPLOG_DBG( "ClusterDestFilePriv::append _next_sequence_num=[%d]",
                           _next_sequence_num );
             
            if( _RWS_ptr == NULL ) { PANIC( "_RWS_ptr is NULL" ); }
            _RWS_ptr->append( *_fd, blox, _writer_id, _next_sequence_num++ );
               // raises ClusterIOException, Service::ServiceException
        }
        catch( clusterio::ClusterIOException & cie )
        {
            // an error occured that requres guidance from NameSpaceService
            cioe_exceptions = &cie.exceptions;
        }

        // if no failure, all done
        if( ! ClusterDestFilePriv::gotFailure( cioe_exceptions)) 
        { return true; }
    }
    return false;
}

// determine if the NameSpaceService considers the contents of the exception
// struct an indication of failure
bool
ClusterDestFilePriv::gotFailure(
    clusterio::IOExceptionStructList * cioe_exceptions
)
{
    // assume failure unless proven otherwise
    clusterio::clusterIOErrorAction erract = clusterio::fail;
    if( cioe_exceptions != NULL )
    {
        erract = _nss->failure( *_fd, *cioe_exceptions );
                                // raises CORBA::SystemException
        
        // update ReadWriteService pointer (first servant in open_info struct)
        // --just in case open_info was modified by the call to failure
        ClusterDestFilePriv::selectReadWriteService();
    }
    else
    {
         return false;
    };  // no exceptions = no failure

    // evaluate response
    switch( erract )
    {
        case clusterio::fail:
            PANIC( "NameSpaceService returned fail" );
            return true;
        case clusterio::retry:
            APPLOG_INFO( "NameSpaceService returned retry" );
            return true;
        case clusterio::success:
            APPLOG_INFO( "NameSpaceService returned success" );
            return false;
        default:
            PANIC( "gotFailure ? clusterIOErrorAction" );
    }
    PANIC( "gotFailure failure state unclear" );
}

void
ClusterDestFilePriv::selectReadWriteService()
{
    // verify that the open_info has servants available for writing to
    if( _fd->servants.length() < 1 )
    {
        PANIC( "ClusterDestFile::selectReadWriteService method called, no servants in list" );
    }

    //
    // using the object's open_info (_fd), do the following:
    // * take first servant
    // * try a write
    // * upon receiving an exception, consult NameSpaceService for direction
    //

    // reference to first servant
    Service::ServiceRef ref = _fd->servants[ 0 ];

    // retrieve the ReadWriteService object corresponding to
    //  the servant referenced
    CORBA::Object_ptr obj =
        SharedServiceMain::instance()->resolve( ref.ior );

    // "narrow" the generic object pointer into its expected form
    _RWS_ptr = clusterio::ReadWriteService::_narrow( obj );
}

//
// ClusterDestFile
//

ClusterDestFile::ClusterDestFile( 
    const clusterio::NameSpaceService_ptr nss,
    const clusterio::writer_id writer_id,
    const char *filename 
)
{
    _helper = new ClusterDestFilePriv( nss, writer_id, filename );

    //first order of business, rollback!
    if( ! _helper->rollback() ) { PANIC( "ClusterDestFile rollback failed" ); }
}

ClusterDestFile::~ClusterDestFile() 
{
    delete _helper;
}

void
ClusterDestFile::put( const char *s ) 
{
    put( s, strlen( s ));
}

// every time put is called, a new block is added to the block sequence
// RWS is only involved on a call to flush
void
ClusterDestFile::put( const char *s, int slen )
{
    // CRC alg
    CRC48HashAlg *crc48factory = CRC48HashAlg::getInstance();

    // declare block and assemble 
    // e_checksum_type         check_type ;
    // checksum_t              checksums ;
    // file_data_t             data ;
    clusterio::block myblock;
    myblock.check_type = clusterio::crc48;
    myblock.checksums.length(1);
    myblock.checksums[0] = crc48factory->getCRC48(
                                reinterpret_cast<unsigned char*>(
                                    const_cast< char* >(s)
                                ), slen
                            );
    myblock.data.replace( slen, slen, reinterpret_cast< CORBA::Octet * >( const_cast< char *>( s )));

    // just 1 block in the block sequence
    clusterio::blockseq myblox( 1, 1, &myblock, 0 );

    _helper->append( myblox );
}

void ClusterDestFile::flush() {
    // append away!
    //_helper->append( *myblox );
    APPLOG_DBG("ClusterDestFile::flush called");
}
#endif
#endif
