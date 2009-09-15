#ident "file_id $Id: SourceFactory.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SourceFactory.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 28 00:57:44 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SourceFactory.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_GLOB_H
#include "frame_config.h"

#include "SourceFactory.h"
#include "BaseException.h"
#include "RawSource.h"
#include "LineSource.h"
#include "RFC822Source.h"
#include "MultiSource.h"
#include "fLineSource.h"
#include "ByteSource.h"
#include "MultiByteSource.h"
#include "FileByteSource.h"
#include "NoCopyDelimitedSource.h"

SourcePtr
SourceFactory::giveback( Source *s )
{
    if( ! s )
    {
        THROW_NOMEM ;
    }
    return SourcePtr(s);
}

DelimitedSourcePtr
SourceFactory::giveback( DelimitedSource *s )
{
    if( ! s )
    {
        THROW_NOMEM ;
    }
    return DelimitedSourcePtr(s);
}


SourcePtr
SourceFactory::getRawSource( const char *filename )
{
    return giveback( new RawSource( filename ));
}

SourcePtr
SourceFactory::getLineSource( const char *filename )
{
    return giveback( new LineSource( filename ));
}

SourcePtr
SourceFactory::getRFC822Source( const char *filename )
{
    return giveback( new RFC822Source( filename ));
}

DelimitedSourcePtr 
SourceFactory::getDelimitedSource(
    const char *filename, const char *sepchars, size_t fields 
)
{
    return giveback( new DelimitedSource( filename, sepchars, fields ));
}

SourcePtr 
SourceFactory::getMultiSource( 
    const char *filename_pattern,
    SourcePtr(*factory)(const char *)
)
{
    glob_t glb;
    memset( &glb, '\0', sizeof(glob_t));
    int flags = FRAME_GLOB_OPTS;
    int ret ;
    SourcePtr ms ( new MultiSource());
    if( 0!=( ret = glob( filename_pattern, flags, NULL, &glb )))
    {
        PANIC_S( "SourceFactory::getMultiSource(): glob failed", 
            filename_pattern );
    }
    if( NULL == ms.get() )
    {
        THROW_NOMEM ;
    }
    for(unsigned int i=0; i< glb.gl_pathc ; ++i )
    {
        ((MultiSource *)(ms.get()))->add( factory( glb.gl_pathv[ i ] ));
    }
    return ms;
}

SourcePtr 
SourceFactory::getMultiSources( 
    list< char_ptr > &patterns,
    SourcePtr(*factory)(const char *)
)
{
    SourcePtr ms ( new MultiSource());
    list< char_ptr >::iterator i = patterns.begin();
    list< char_ptr >::iterator e = patterns.end();
    for( ; i!=e; ++i )
    {
        ((MultiSource *)(ms.get()))->add( 
            getMultiSource( (*i).get(), factory )
        );
    }
    return ms;
}

DelimitedSourcePtr 
SourceFactory::getDelimitedMultiSource(
    const char *filename_pattern, const char *sepchars, size_t fields 
)
{
    //TODO dont hardcode bufsize
    DelimitedSourcePtr dsp( new fLineSource( filename_pattern, 257 * 1024, fields, *sepchars ));
    return dsp;
}

DelimitedSourcePtr 
SourceFactory::getDelimitedMultiSources(
    list< char_ptr> &patterns, const char *sepchars, size_t fields 
)
{
    //TODO dont hardcode bufsize
    DelimitedSourcePtr dsp( new fLineSource( patterns, 257 * 1024, fields, *sepchars ));
    return dsp;
}

SourcePtr 
SourceFactory::getRawMultiSource(const char *filename_pattern )
{
    return getMultiSource( filename_pattern, SourceFactory::getRawSource );
}

SourcePtr 
SourceFactory::getLineMultiSource(const char *filename_pattern )
{
    return getMultiSource( filename_pattern, SourceFactory::getLineSource );
}

SourcePtr 
SourceFactory::getRFC822MultiSource(const char *filename_pattern )
{
    return getMultiSource( filename_pattern, SourceFactory::getRFC822Source );
}

DelimitedSourcePtr 
SourceFactory::getNoCopyDelimitedMultiSources(
    list< char_ptr> &patterns, char sepchar, size_t fields, size_t bufsize
)
{
    ByteSourcePtr bs ( new MultiByteSource());
    MultiByteSource *mbs = dynamic_cast< MultiByteSource *>( bs.get());
    int flags = FRAME_GLOB_OPTS;
    glob_t glb;
    list< char_ptr >::iterator i = patterns.begin();
    list< char_ptr >::iterator e = patterns.end();
    for( ; i!=e; ++i )
    {
        memset( &glb, '\0', sizeof(glob_t));
        int ret ;
        if( 0!=( ret = glob( i->get(), flags, NULL, &glb )))
        {
            if( ! strcmp( i->get(), "-" ))
            {
                ByteSourcePtr dash( new FileByteSource( i->get() ));
                mbs->add( dash );
                continue;
            } 
            else
            {
                PANIC_S( "SourceFactory::getZeroCopyMultiSource(): glob failed", i->get() );
            }
        }
        for(unsigned int j=0; j< glb.gl_pathc ; ++j )
        {
            ByteSourcePtr addbs( new FileByteSource( glb.gl_pathv[ j ] ));
            mbs->add( addbs );
        }
    }
    DelimitedSourcePtr dsp( new NoCopyDelimitedSource( bs, bufsize, fields, sepchar ));
    return dsp;
}

