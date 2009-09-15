#ident "file_id $Id: CplusqlMain.cpp,v 1.10 2005/10/17 11:18:58 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CplusqlMain.cpp
// Author:    mgrosso
// Created:   Wed Apr 16 23:46:26 EDT 2003 on relay.looksmart.com
// Project:
// Purpose:
//
// Copyright (c) 2003 LookSmart.
//
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA
//
// $Id: CplusqlMain.cpp,v 1.10 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "keys.h"
#include "cplusql_api.h"
#include "CplusqlContext.h"
#include "CplusqlExceptionHandling.h"
#include "fwrap.h"
#include "Config.h"
#include "ptr_strdup.h"
#include "Parse.h"
#include "AppLog.h"

extern char *optarg;
extern FILE *yyin ;

void usage()
{
    fprintf(stderr,"%s", CplusqlContext::getUsageString());
    exit(1);
};

void
parse_from_to( char *fromto, char **ns, char **file )
{
    if(!fromto)
    {
        PANICV("null arg");
    }
    ptr< vector< char *> > pfrom = Parse::parse_ptr( fromto, ":", false );
    if( pfrom->size() != 2 )
    {
        PANICV("usage: need exactly one \':\' in --{from|to}={$namespace|file|}:$path");
    }
    *file = (*pfrom)[1] ;
    if( ** file  == '\0' )
    {
        PANICV("usage: need path in --{from|to}={$namespace|file|}:$path");
    }
    *ns = (*pfrom)[0] ;
    if( !strcasecmp( *ns, CPLUSQL_USE_FILE_SYSTEM ))
    {
        *ns = strdup_wnew(NSS_USE_FILE_SYSTEM);
    }
    else if( *ns  == '\0' )
    {
        *ns = strdup_wnew(CPLUSQL_NAMESPACE_DFLT);
    }
}


int main( int argc, char **argv )
{
    try{
        //if(!CplusqlContext::instance().initConfig( &argc, argv ))
        if(!cplusql_init_default_cfg( &argc, argv ))
        {
            usage();
        }
        const char *ns="ns";
        const char *ls=cplusql_getvar( "ls" );
        const char *find=cplusql_getvar( "find" );

        const char *cat=cplusql_getvar( "cat" );
        const char *from=cplusql_getvar( "from" );
        const char *to=cplusql_getvar( "to" );

        const char *nss_ior = cplusql_getvar( NSS_IOR_URI );
        const char *nss_url = cplusql_getvar( NSS_URL );
        const char *ns_host = cplusql_getvar( NS_HOST );
        const char *ns_port = cplusql_getvar( NS_PORT );

        if( nss_ior or nss_url or ( ns_host and ns_port ))
        {
            //cplusql_namespace( cplusql_string(ns), cplusql_default_cfg());
        }
        if( ls || find )
        {
            const char *prefix =find;
            char_ptr fp = ptr_strdup(".*");
            if( ls )
            {
                prefix = ls ;
                fp = ptr_strdup("^");
                fp = fp + ls ;
                fp = fp + "$" ;
            }
            cplusql_namespace_find(
                cplusql_string( ns ),
                cplusql_string( prefix ),
                cplusql_string( cplusql_getvar_wdflt( "PATTERN", fp.get())),
                cplusql_default_cfg());
        }
        else if( cat )
        {
            const char *catsource = "catsource" ;
            char *from_file = NULL;
            char *from_ns = NULL;

            if( from )
            {
                parse_from_to(cplusql_string(from), &from_ns, &from_file );
            }
            else
            {
                from_file = cplusql_string("-") ;
            }
            cplusql_byte_source(
                cplusql_string( catsource ),
                cplusql_new_string_set(
                    cplusql_string( from_file )),
                from_ns ?
                    cplusql_start_config(
                        cplusql_string(CPLUSQL_NAMESPACE), 
                        cplusql_string(from_ns)) :
                    cplusql_empty_config());

            char *to_file = NULL;
            char *to_ns = NULL;
            if( to )
            {
                parse_from_to(cplusql_string(to),&to_ns,&to_file );
            }
            else
            {
                to_file = cplusql_string("-") ;
            }
            cplusql_dest_file(
                cplusql_string( catsource ),
                cplusql_string( to_file ),
                to_ns ?
                    cplusql_start_config(
                        cplusql_string(CPLUSQL_NAMESPACE), 
                        cplusql_string(to_ns)) :
                    cplusql_default_cfg());
        }
        else
        {
            yyin=fwrap::fopen( cplusql_getvar_wdflt( "file","-" ), "r" );
            do{
                cplusql_yyparse();
            }while( !feof(yyin));
        }
        cplusql_run();
        CplusqlContext::instance().exit();
        return 0;
    }CPLUSQL_CATCHALL();
    //not reached.
    return 1;
};

