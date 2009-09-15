#ident "file_id $Id: ConfigDump.cpp,v 1.4 2004/01/05 20:21:28 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ConfigDump.cpp
// Author:    Matt Grosso
// Created:   Mon Nov 16 02:45:28 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: ConfigDump.cpp,v 1.4 2004/01/05 20:21:28 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <unistd.h>
#include "ConfigDump.h"
#include "ConfigPriv.h"

typedef vector< pair< string, string > >::iterator vpsitr_t ;

void 
wstr( string &s, int fd ){
    if(-1==write( fd, s.c_str(), s.size() ))
        perror( __FILE__ );
};

pConfigDumpT
ConfigDump::dump( const Config &cf ){
    constcmapi_t itr;
    constcmapi_t end;
    pConfigDumpT gb( new ConfigDumpT() );
    for( itr=cf.d_->cmap_.begin(), end=cf.d_->cmap_.end();
        itr!=end; 
        ++itr ){
        
        gb->push_back(
            pair<string, string>( (*itr).first, (*itr).second )
        );
    }
    return gb;
};

void
ConfigDump::write( const Config &cf, int fd ){
    pConfigDumpT dp = dump( cf );
    vpsitr_t itr;
    vpsitr_t end;
    string equals("=");
    string endl("\n");
    for( itr=dp->begin(), end=dp->end(); itr!=end;  ++itr ){
        wstr( (*itr).first, fd  );
        wstr( equals, fd );
        wstr( (*itr).second, fd  );
        wstr( endl, fd );
    };
};

void
ConfigDump::dumpToString( const Config &cf, string &gb ){
    pConfigDumpT dp = dump( cf );
    vpsitr_t itr;
    vpsitr_t end;
    string equals("=");
    string endl("\n");
    for( itr=dp->begin(), end=dp->end(); itr!=end;  ++itr ){
        gb.append( (*itr).first);
        gb.append( equals);
        gb.append( (*itr).second);
        gb.append( endl);
    };
};
