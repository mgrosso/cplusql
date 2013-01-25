#ident "file_id $Id: DestHashFiles.cpp,v 1.13 2004/12/31 20:57:29 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestHashFiles.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Tue Oct 29 04:24:08 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DestHashFiles.cpp,v 1.13 2004/12/31 20:57:29 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
using namespace std;
#include "Dest.h"
#include "DestHashFiles.h"
#include "DestFile.h"
#include "DestBuffered.h"
#include "DestThreadedBuffer.h"
#include "AbstractHashAlg.h"
#include "AbstractDestFactory.h"
#include "HashAlgFactory.h"
#include "ptr_strdup.h"
#include "hashFuncs.h"
#include "Parse.h"
#include "frame_keys.h"

#include <stdio.h>
#include <map>
#ifndef FILENAME_MAX
//should be almost 4k, but if FILENAME_MAX is not provided by stdio.h, then
//we make a more conservative assumption.
#define FILENAME_MAX 1024
#endif

typedef map< const char *, pDest, cc_less_than >  mcpd_t;

class DestHashFilesPriv
{
    private:
    friend class DestHashFiles;
    list< char_ptr >            names_ ;//list ensures that const char * in mdest 
                                        //continue to point somwhere 
    mcpd_t                      mdest_;

    ptr< HashHelper >       	hasher_ ;
    ptr< AbstractDestFactory >  destFactory_;
    char_ptr                    template_ ;
    ConfigPtr                   cfg_ ;
    bool                        onefd_ ;

    DestHashFilesPriv( 
        ptr< HashHelper >       	hasher,
        ptr< AbstractDestFactory >  destFactory,
        const char *                templ,
        ConfigPtr &                 cfg
    );
};

DestHashFilesPriv::DestHashFilesPriv( 
    ptr< HashHelper >       	hasher,
    ptr< AbstractDestFactory >  destFactory,
    const char *                templ,
    ConfigPtr &                 cfg
)
:
    hasher_(hasher),
    destFactory_(destFactory), 
    template_(ptr_strdup(templ)), 
    cfg_( new Config( *(cfg.get()))),
    onefd_(false)
{
    ;//noop
}

DestHashFiles::DestHashFiles(
    ptr< HashHelper > hashhelper,
    ptr< AbstractDestFactory > destFactory,
    const char *templ, 
    ConfigPtr & cfg
)
{
    d_=new DestHashFilesPriv(hashhelper,destFactory,templ, cfg);
    if( NULL==strstr( templ, "%s" ))
    {
        PANIC( "DestHashFiles:: ctor(): filename_pattern must contain %%s.");
    }
    //by default, we dont want to use thread per dest, since we may more dests
    //than we can have threads.
    (void) d_->cfg_->addString(DEST_THREADED, "0", true );
    //APPLOG_DBG( "DEST_THREADED %s, %i", d_->cfg_->
    if( cfg->getBool( DEST_HASH_ONEFD, "0" ))
    {
        d_->onefd_ = true;
    }
    else if(cfg->findString(DEST_HASH_BUCKET_MIN) && cfg->findString(DEST_HASH_BUCKET_MAX))
    {
        long long min = cfg->getInt(DEST_HASH_BUCKET_MIN);
        long long max = cfg->getInt(DEST_HASH_BUCKET_MAX);
		if(cfg->findString(DEST_HASH_INIT_MAX_BUCKET))
		{
			if(!cfg->getBool(DEST_HASH_INIT_MAX_BUCKET))
				--max;
		}
	    if(min > max)
	    {
	        PANICV("Config value of %lli is greater then the value of %lli", 
                DEST_HASH_BUCKET_MIN, DEST_HASH_BUCKET_MAX);
	    }
	    for(long long i=min; i <= max; ++i)
	    {
	        char_ptr bucket ( ptr_strdup_longlong( i ) );
			getDest_( bucket );
		}
    }
	else if(cfg->findString(DEST_HASH_BUCKET_LIST))
	{
		char_ptr bucket_list = ptr_strdup(cfg->getString(DEST_HASH_BUCKET_LIST));
		ptr< vector< char *> > list = Parse::parse_ptr( bucket_list.get(), ":", false );
		vector<char *>::iterator beg;
		vector<char *>::iterator end = list->end();
		for( beg = list->begin(); end != beg; ++beg)
		{
			char_ptr bucket = ptr_strdup(*beg);
			getDest_( bucket );
		}
	}
}


pDest
DestHashFiles::getDest()
{
    char_ptr s=d_->hasher_->getHash();
	return getDest_(s);
}

DestHashFiles::~DestHashFiles()
{
    delete d_;
}

void
DestHashFiles::put( const char *s )
{
    if( d_->onefd_ )
    {
        pDest d = getDest();
        d->put( s );
        d->flush();
    }
    else
    {
        getDest()->put( s );
    }
}

void
DestHashFiles::put( const char *s, int len )
{
    if( d_->onefd_ )
    {
        pDest d = getDest();
        d->put(s, len );
        d->flush();
    }
    else
    {
        getDest()->put( s, len );
    }
}

void
DestHashFiles::flush( void )
{
    mcpd_t::iterator i=d_->mdest_.begin();
    mcpd_t::iterator e=d_->mdest_.end();
    //it would be nice to use the stl algorithm, for_each, like such...
    //for_each( d_->mdest_.begin(), d_->mdest_.end(), mem_fun( hmmmm...));
    //but while using smart pointers is cool, it means that we would have to 
    //construct a special functor just for this, which just aint worth it.
    for( ; i!=e; ++i )
    {
        (*i).second->flush();
    }
}

pDest
DestHashFiles::getDest_( char_ptr & bucket )
{
    if( d_->onefd_ )
    {
        pDest raw = d_->destFactory_->getFileDest( bucket.get());
        pDest buffered( new DestBuffered( 1024, raw, false ));
        return buffered;
    }
    mcpd_t::iterator desti;
    mcpd_t::iterator deste=d_->mdest_.end();
    desti=d_->mdest_.find( bucket.get() );
	if( desti==deste )
	{
		d_->names_.push_front( bucket );

		char filename[FILENAME_MAX];
		memset(filename,'\0', FILENAME_MAX );
		snprintf(filename,FILENAME_MAX,d_->template_.get(),bucket.get());
        pDest f = d_->destFactory_->getFileDest( filename );
        pDest b = DestThreadedBuffer::factory( f, d_->cfg_ );
        return (
            *(
                d_->mdest_.insert(
                    pair<const char *, pDest>(
                        bucket.get(), b
                    )
                ).first
            )
        ).second;
	}
    return (*desti).second ;
}
