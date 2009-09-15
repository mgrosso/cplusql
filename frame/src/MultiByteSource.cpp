#ident "file_id $Id: MultiByteSource.cpp,v 1.7 2004/02/07 19:37:46 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MultiByteSource.cpp
// Author:    mgrosso 
// Created:   Sun Apr 27 17:43:23 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: MultiByteSource.cpp,v 1.7 2004/02/07 19:37:46 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_GLOB_H
#include "frame_config.h"
#include "MultiByteSource.h"
#include "BaseException.h"
#include "FileByteSource.h"
#include "fwrap.h"
#include <list>

typedef list< ByteSourcePtr > lByteSourcePtr ;

class MultiByteSourcePriv 
{
    private:
    friend class MultiByteSource ;
    lByteSourcePtr          sources_;
    MultiByteSourcePriv()
    {
        //noop
    };
};

MultiByteSource::MultiByteSource()
{
    d_=new MultiByteSourcePriv ();
}

MultiByteSource::~MultiByteSource()
{
    delete d_;
}

bool    
MultiByteSource::next( char *buffer, size_t &bytes_read, size_t bufsize )
{
    bool gb = false;
    while( !gb && ! d_->sources_.empty())
    {
        if( gb = d_->sources_.front()->next( buffer, bytes_read, bufsize ))
        {
            return gb;
        }
        d_->sources_.pop_front();
    }
    return false;
}

void            
MultiByteSource::add( ByteSourcePtr &bsp )
{
    d_->sources_.push_back( bsp );
}

void            
MultiByteSource::add( list< char_ptr> &patterns )
{
    flist_t files = fwrap::fglobs( patterns, "r" );
    flist_t::iterator i;
    flist_t::iterator e;
    for( i=files.begin(), e=files.end(); i!=e; ++i )
    {
        ByteSourcePtr bsp( new FileByteSource( fileno( *i ), true ));
        add( bsp );
    }
}

ByteSourcePtr
MultiByteSource::makeFromGlobs(
    list< char_ptr> &patterns
)
{
    MultiByteSource *mbs =new MultiByteSource();
    ByteSourcePtr bs ( mbs );
    mbs->add( patterns );
    return bs;
}

