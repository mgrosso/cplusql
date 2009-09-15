#ident "file_id $Id DestFile.cpp,v 1.4 2003/10/28 22:22:42 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestFile.cpp
// Author:    mgrosso 
// Created:   Mon Jul  8 11:12:28 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: DestFile.cpp,v 1.10 2004/12/31 21:41:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "frame_config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /*  HAVE_UNISTD_H */

#include "DestFile.h"
#include "BaseException.h"
#include "fwrap.h"


#include "dbg.h"

DestFile::DestFile( const char *filename )
:fd_(fwrap::open_write(filename)), close_on_delete_(true)
{
    ;//noop
}

DestFile::DestFile( const char *filename, bool close_on_delete )
:fd_(fwrap::open_write(filename)), close_on_delete_(close_on_delete)
{
    ;//noop
}

DestFile::DestFile( const char *filename, bool close_on_delete, bool o_excl, bool o_trunc )
:fd_(fwrap::open_write(filename, o_excl, o_trunc )), close_on_delete_(close_on_delete)
{
    ;//noop
}

DestFile::DestFile( int fd )
:fd_(fd), close_on_delete_(false)
{
    ;//noop
}

DestFile::DestFile( int fd, bool close_on_delete )
:fd_(fd), close_on_delete_(close_on_delete)
{
    ;//noop
}

DestFile::~DestFile()
{
    //fprintf(stderr, "DestFile::~DestFile()\n" );
    if(fd_>-1 && close_on_delete_ )
    {
        close(fd_);
    }
}

void
DestFile::put( const char *s )
{
    put( s, strlen(s));
}

void
DestFile::put( const char *s, int slen )
{
    //fprintf(stderr, "DestFile::put(), writing %i bytes\n", slen );
    int rt = fwrap::write( fd_, s, slen, MAX_CONSECUTIVE_EINTR );
    if(rt<0)
    {
        //should expand to handle EINTR, where not SIGPIPE, 
        PANICV("DestFile::put() cant write to file: fd_=%i", fd_ );
    }
    if( rt != slen  )
    {
        PANICV("did not write full amount: wrote %lu of %i to fd_=%i", 
            rt, slen, fd_ );
    }
}

void
DestFile::flush( void )
{
   FDBG_S("DestFile::flush( void )");
   int i=fsync( fd_ );
   if( i != 0 && errno != EROFS && errno != EINVAL )
   {
       PANIC("DestFile::flush() failed");
   }
}

