#ident "file_id $Id: DestBuffered.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestBuffered.cpp
// Author:    mgrosso 
// Created:   Sun Jul  7 23:45:39 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: DestBuffered.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include <string.h>
#include "Dest.h"
#include "DestBuffered.h"
#include "BaseException.h"
#include "dbg.h"

class DestBufferedPriv
{
    private:
    friend class DestBuffered;
    DestBufferedPriv( int bufsize, pDest &d, bool newline )
    :
        bufsize_(bufsize), dest_(d), newline_(newline),
        buffer_(new char [bufsize])
    {
        memset(buffer_, '\0', bufsize_);
        end_=buffer_+bufsize_;
        cursor_=buffer_;
    };
    int   bufsize_;
    pDest dest_;
    bool  newline_;

    char *buffer_;
    char *end_; 
    char *cursor_;      //cursor_ must always point at a valid byte in buffer
};

DestBuffered::DestBuffered( int bufsize, pDest &d, bool newline )
{
    d_= new DestBufferedPriv( bufsize, d, newline );
}

DestBuffered::~DestBuffered()
{
    FDBG_S("DestBuffered::~DestBuffered()");
    flush();
    d_->dest_->flush();
    delete [] d_->buffer_;
    delete d_ ;
}

void
DestBuffered::flush()
{
    if( *d_->buffer_ != '\0' )
    {
        FDBG_S("DestBuffered::flush() flushing");
        d_->dest_->put(d_->buffer_, d_->cursor_ - d_->buffer_ );
        d_->dest_->flush();
    }
    memset(d_->buffer_, '\0', d_->bufsize_);
    d_->cursor_=d_->buffer_;
}

void 
DestBuffered::put( const char *s )
{
    if( ! s )
    {
        PANIC( "DestBuffered::put() null string " );
    }
    //prior to this call, cursor_ must point to valid byte of buffer_.
    //
    //after this call, cursor_ will still point to a valid byte of buffer_.
    //
    //end_ is one past the edge of the buffer, so if d_->cursor_ is past it, then
    //we need to flush(). flush() takes care of resetting cursor_.

    //we loop until we get to null terminator of s.
    //we dont advance the cursor unless the value of c is not '\0'
    char c;
    while((c = *s++ ))  
    {
        *d_->cursor_++=c;
        if( d_->cursor_ == d_->end_ )
        {
            flush();
        }
    }
}

void 
DestBuffered::put( const char *s, int lens )
{
    put(s);
    if( d_->newline_ )
    {
        put( "\n" );
    }
}

