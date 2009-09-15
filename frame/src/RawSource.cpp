#ident "file_id $Id: RawSource.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RawSource.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Fri Sep  6 00:34:14 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: RawSource.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RawSource.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "BaseException.h"
#ifdef  O_LARGEFILE 
#define RS_LARGEFILE_FLAG O_LARGEFILE
#else
#define RS_LARGEFILE_FLAG 0
#endif

class RawSourcePriv
{
    friend class RawSource;
    private:
        int block_size;
        int fd;
        int line ;
        char *buf;
        bool use_memset ;
};

RawSource::~RawSource()
{
    if(d_)
    {
        close(d_->fd);
        if( d_->buf)
        {
            delete [] d_->buf;
        }
        delete d_;
    }
}

void
RawSource::init( int fd, int block_size, bool use_memset )
{
    d_=new RawSourcePriv();
    if(!d_)
    {
        THROW_NOMEM;
    }
    d_->block_size = block_size;
    d_->fd = fd;
    d_->use_memset = use_memset;
    d_->buf = NULL;
    d_->line = 0;
}

int
RawSource::init_file( const char *filename )
{
    if(!filename )
    {
        PANIC("RawSource::init_file(): null filename pointer");
    }
    if( !strcmp( filename , "-" ))
    {
        return 0;
    }
    int fd = open( filename, O_RDONLY | RS_LARGEFILE_FLAG );
    if( fd==-1 )
    {
        PANIC_S("cant open file", filename );
    }
    return fd;
}

RawSource::RawSource( int fd, int block_size, bool use_memset )
{
    init( fd, block_size, use_memset );
}

RawSource::RawSource( const char *filename, int block_size, bool use_memset )
{
    init( init_file(filename), block_size, use_memset );
}

RawSource::RawSource( const char *filename )
{
    init( init_file(filename), DEFAULT_BLOCK_SIZE, DEFAULT_USE_MEMSET );
}

bool 
RawSource::next()
{
    ++ d_->line;
    if(NULL==d_->buf)
    {
        d_->buf=new char [d_->block_size] ;
        if(!d_->buf)
        {
            THROW_NOMEM;
        }
    }
    if( d_->use_memset )
    {
        memset( d_->buf, '\0', d_->block_size );
    }
    int eintr_count = 0 ;
    int bytes_read = 0;
    while( bytes_read < d_->block_size )
    {
        //note stupid (void *)((char *)) cast to shut up compiler
        ssize_t ret = read( 
            d_->fd, 
            (void *)((char *)d_->buf + bytes_read), 
            d_->block_size - bytes_read 
        );
        if( ret > 0 )//successfull read, but maybe not full block
        {
            bytes_read += ret ;
            eintr_count =0;
        }
        else if( ret == 0 )//end of file
        {
            if( bytes_read > 0 ) //previous read got partial block.
            {
                return true;// do I need to set and check some flag here?
            }
            else
            {
                return false;
            }
        }
        else if( ret < 0 )
        {
            if( errno != EINTR  )
            {
                PANIC( "read error");
            }
            ++eintr_count ;
            if( eintr_count >= MAX_CONSECUTIVE_EINTR )
            {
                //eintr_count works around a bug in which epipe isnt returned
                //for a broken pipe. this can happen if there is a signal handler
                //installed for sigpipe. when that happens, code that checks for
                //eintr can be rewarded with an infinite loop for its troubles.
                PANIC( "read error");
            }
        }
    }
    return true;
}

char_ptr    
RawSource::getCharPtr()
{
    //zero copy
    char_ptr giveback( d_->buf );
    d_->buf = NULL;
    return giveback;
}

int         
RawSource::lineNum()
{
    return d_->line;
}

