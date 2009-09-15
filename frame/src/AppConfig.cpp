#ident "file_id $Id: AppConfig.cpp,v 1.4 2004/08/04 23:02:38 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AppConfig.cpp
// Author:    Matt Grosso
// Created:   Tue Mar 23 01:21:24 EST 1999 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Matt Grosso.  All Rights Reserved.
// 
// $Id: AppConfig.cpp,v 1.4 2004/08/04 23:02:38 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <vector>
#include <string>
#include "AppConfig.h"
#include "Parse.h"

AppConfig * AppConfig::instance_ = NULL; 

AppConfig *
AppConfig::instance()
{
    if(!instance_)
    {
        instance_=new AppConfig();
    }
    return instance_ ;
}


void
AppConfig::shutdown()
{
	if (instance_ != (AppConfig *) 0)
	{
		delete instance_;
		instance_ = (AppConfig *) 0;
	}
}


void 
AppConfig::init( int argc, char **argv )
{
    //first apply env.
    loadEnv( true );

    //now apply config files in order, each one overrides any previous
    Config c;
    c.loadEnv( true );
    c.loadArgs( argc, argv, true );
    const char *f = c.findString( APPCONFIG_FILES_KEY );
    if( f )
    {
        vector< string >::iterator i;
        vector< string >::iterator e;
        vector< string > files = Parse::parseString( f, "," );
        for( i=files.begin(), e=files.end(); i!=e; ++i )
        {
            slurp( (*i).c_str(), true );
        }
    }

    //now apply command line arguments last, as they trump everything.
    loadArgs( argc, argv, true );
}

