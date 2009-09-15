#ident "file_id $Id: FileByteSource.cpp,v 1.9 2004/12/31 21:44:28 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FileByteSource.cpp
// Author:    mgrosso 
// Created:   Sun Apr 27 02:03:34 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: FileByteSource.cpp,v 1.9 2004/12/31 21:44:28 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "FileByteSource.h"
#include "fwrap.h"
#include "BaseException.h"
#include "AppLog.h"

#include <unistd.h>

class FileByteSourcePriv
{
private:
    friend class FileByteSource ;

    FileByteSourcePriv( int fd, bool close_on_delete );
    FileByteSourcePriv( int fd, ewproc_t *ew );
    int                 fd_;
    ewproc_t *          ew_;
    bool                close_on_delete_ ;
};

FileByteSourcePriv::FileByteSourcePriv( int fd, bool close_on_delete )
:fd_(fd),ew_(NULL),close_on_delete_(close_on_delete)
{
    ;//
}

FileByteSourcePriv::FileByteSourcePriv( 
        int fd, ewproc_t *ew )
:fd_(fd),ew_(ew),close_on_delete_(true)
{
    ;//
}


FileByteSource::FileByteSource( int fd, ewproc_t *ew )
{
    d_=new FileByteSourcePriv( fd, ew );
}

FileByteSource::FileByteSource( const char *filename )
{
    d_=new FileByteSourcePriv( fwrap::open_read( filename), true );
}

FileByteSource::FileByteSource( int fd )
{
    d_=new FileByteSourcePriv( fd, false );
}

FileByteSource::FileByteSource( int fd, bool close_on_delete )
{
    d_=new FileByteSourcePriv( fd, close_on_delete );
}

FileByteSource::~FileByteSource()
{
    if( d_->close_on_delete_ )
    {
        close(d_->fd_);
    }
    delete d_ ;
}

bool 
FileByteSource::next( char *buffer, size_t &bytes_read, size_t bufsize )
{
    //APPLOG_DBG("called with buf=%i", bufsize );
    
    if(!buffer)
    {
        PANIC("FileByteSource::next(), null buffer");
    }
    bytes_read = 0;

    int br = fwrap::read( d_->fd_, (buffer + bytes_read), bufsize - bytes_read, MAX_CONSECUTIVE_EINTR);
    if( br > 0 )//common case, successfull read, but maybe not full block
    {
        bytes_read = br ;
        return true;
    }
    else if( br == 0 )//end of file
    {
        return false;
    }
    else //( br < 0 )
    {
        //check if we are reading from a process and it has exited.
        if(
            ( errno == EINTR || errno == EPIPE ) && 
            d_->ew_ && 
            d_->ew_->result != -1 
        )
        {
            return false;
        }
        if( errno != EINTR  )
        {
            PANIC( "read error");
        } 
        PANIC( "read error: got EINTR too many times in a row, breaking loop"); 
    }
    //not reached.
    return true;
}

