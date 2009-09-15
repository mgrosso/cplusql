// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ConfigDump.h
// Author:    Matt Grosso
// Created:   Mon Nov 16 02:37:15 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: ConfigDump.h,v 1.3 2002/07/15 18:35:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CONFIGDUMP_H
#define CONFIGDUMP_H 1

// this functionality is separated into a separate file so that not all
// clients of Config need to compile and link in knowledge of vectors
// of pairs of basic_string of char!

#include <vector>
#include <string>
#include "Config.h"
#include "ptr.h"

typedef vector< pair< string, string > > ConfigDumpT ;
typedef ptr< ConfigDumpT > pConfigDumpT ;

class ConfigDump {
    public:
    static pConfigDumpT dump( const Config &cf );
    static void write( const Config &cf, int fd );
    static void dumpToString( const Config &cf, string &gb );
};

#endif /* CONFIGDUMP_H */
