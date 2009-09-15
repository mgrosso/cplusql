#ident "file_id $Id: ClusterDestBuffered.cpp,v 1.2 2005/10/17 11:18:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      
// Author:    
// Created:  
// Project:   
// Purpose:  different tact when buffering clusterio output --
//           since each flush must end on a newline, the flush is
//           triggered only when the bufsize threshold is crossed
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: ClusterDestBuffered.cpp,v 1.2 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "cplusql_config.h"
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1

#include <string.h>
#include "ClusterDestFile.h"
#include "ClusterDestBuffered.h"
#include "BaseException.h"
#include "dbg.h"
#include "AppLog.h"

class ClusterDestBufferedPriv
{
    private:
    friend class ClusterDestBuffered;
    ClusterDestBufferedPriv( int bufsize, pDest &d, bool newline )
    : bufsize_(bufsize),dest_(d),newline_(newline),bufbytes_(0)
    {
    };
    int   bufsize_;
    int   bufbytes_;
    bool  newline_;
    pDest dest_;
};

ClusterDestBuffered::ClusterDestBuffered( int bufsize, pDest &d, bool newline )
{
    d_= new ClusterDestBufferedPriv( bufsize, d, newline );
}

ClusterDestBuffered::~ClusterDestBuffered()
{
    flush();
    d_->dest_->flush();
    delete d_ ;
}

void
ClusterDestBuffered::flush()
{
    APPLOG_DBG("ClusterDestBuffered::flush() flushing");
    d_->dest_->flush();
    d_->bufbytes_ = 0;
}

void 
ClusterDestBuffered::put( const char *s )
{
    if( ! s )
    {
        PANIC( "ClusterDestBuffered::put() null string " );
    }

    // automatically pass through the string (assumes terminated properly!)
    d_->dest_->put( s );

    // increment bufbytes by the length of the string
    d_->bufbytes_ += strlen( s );
    
    // if bufbytes has equaled or exceeded the bufsize, flush
    if( d_->bufbytes_ >= d_->bufsize_ )
    {
        flush();
    }
}

void 
ClusterDestBuffered::put( const char *s, int lens )
{
    put(s);
    if( d_->newline_ )
    {
        put( "\n" );
    }
}

#endif
#endif
