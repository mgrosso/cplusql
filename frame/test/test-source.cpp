#ident  "file_id $Id: test-source.cpp,v 1.2 2004/01/05 20:06:24 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-source.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 28 16:25:56 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: test-source.cpp,v 1.2 2004/01/05 20:06:24 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------



#include "Source.h"
#include "MultiSource.h"
#include "SourceFactory.h"
#include "BaseException.h"
#include "Dest.h"
#include <list>


void 
push( SourcePtr &sp, pDest &pd )
{
    while( sp->next())
    {
        char_ptr cp=sp->getCharPtr();
        pd->put( cp.get());
    }
};

int main( int argc, char **argv )
{
    try{
        {
            SourcePtr spl = SourceFactory::getRawSource( "/etc/passwd" );
            pDest pdl= DestFactory::getFileDest( "test-source-etc-pwd" );
            push( spl, pdl );
        }
        {
            SourcePtr spl2 = SourceFactory::getLineSource( "/etc/passwd" );
            pDest pdl2= DestFactory::getFileDest( "test-source-etc-pwd2" );
            push( spl2, pdl2 );
        }
        {
            SourcePtr spl3 = SourceFactory::getLineSource( "/etc/passwd" );
            pDest pdl3= DestFactory::getBufferedFileDest( "test-source-etc-pwd3", true );
            push( spl3, pdl3 );
        }
        {
            SourcePtr sp2 = SourceFactory::getLineMultiSource( 
                "/etc/rc.d/init.d/*" 
            );
            pDest pdrcd= DestFactory::getBufferedFileDest( "test-source-etc-rcd", true );
            push( sp2, pdrcd );
        }
    }catch( BaseException &be )
    {
        fprintf( stderr, "caught BaseException in main: %s", 
            be.getMessage()
        );
        return 1;
    }
    return 0;
}

