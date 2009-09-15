#ident  "file_id $Id: test-config.cpp,v 1.6 2004/09/22 17:35:56 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      cfmain.cpp
// Author:    Matt Grosso
// Created:   Thu Nov 12 22:59:16 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: test-config.cpp,v 1.6 2004/09/22 17:35:56 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdio.h>
#include "Config.h"
#include "ConfigDump.h"

void prnt( Config &cf, const char *key ){
    if(!key) key="NULL_PTR";
    const char *val = cf.getString( key );
    fprintf(stderr,"%s=%s\n",key,val==NULL? "null":val );
};

void prnti( const char *key, const char *def, int i ){
    fprintf(stderr,"%s,%s -> %i\n",key,def,i);
};
void prntb( const char *key, const char *def, bool b ){
    fprintf(stderr,"%s,%s -> %i\n",key,def,b);
};
void prntd( const char *key, const char *def, double d ){
    fprintf(stderr,"%s,%s -> %g\n",key,def,d);
};

void dumpKeyValues(Config &cf) {
	char_ptr_vec *keys = cf.getKeys();

	fprintf(stderr, "\nDumping all keys:\n--------\n");

	for (size_t i = 0; i < keys->size(); i++) {
		const char *key = (*keys)[i].get();

		prnt(cf, key);
	}
	fprintf(stderr, "--------\n\n");
	delete keys;
}


int main( int argc, char **argv ){
    try {
        Config cf;
        cf.addString( "foo", "bar", true );
        prnt( cf, "foo");
        cf.addString( "foo", "fubar", true );
        prnt( cf, "foo");
        cf.addString( "foo", "fucked", false );
        prnt( cf, "foo");
        cf.addString( "bar", "barbara annnn", false );
        prnt( cf, "bar");

        dumpKeyValues(cf);

        Config cf2;
        cf2.addString( "bar", "HUBBA", false );
        cf2.addString( "foo", "BUBBA", true );
        cf2.addString( "fu", "BACKIP", true );
        cf.slurp( cf2, false );
        prnt( cf, "foo");
        prnt( cf, "bar");
        prnt( cf, "fu");
        cf.slurp( cf2, true );
        prnt( cf, "foo");
        prnt( cf, "bar");
        prnt( cf, "fu");

        dumpKeyValues(cf2);
        dumpKeyValues(cf);

        Config cf3( "./foo.cfg" );
        ConfigDump::write( cf3, 1 );
        cf3.slurp( "./foo2.cfg", false );
        ConfigDump::write( cf3, 1 );
        cf3.slurp( "./foo2.cfg", true );
        ConfigDump::write( cf3, 1 );

        dumpKeyValues(cf3);

        prnti( "fungi", "1", cf3.getInt( "fungi", "1" ));
        prnti( "fungi", "2.2", cf3.getInt( "fungi", "2.2" ));
        prntb( "fungb", "0", cf3.getBool( "fungb", "0" ));
        prntb( "fungb", "1", cf3.getBool( "fungb", "1" ));
        prntd( "fungd", "1.5", cf3.getDouble( "fungd", "1.5" ));
        prntd( "fungd", "1.6", cf3.getDouble( "fungd", "1.6" ));
        {
            //fprintf(stderr, "starting loop to find memloss");
            //for( int i=1000000; i; --i )
            for( int i=100; i; --i )
            {
                Config typical;
                typical.loadEnv( true );
                typical.loadArgs( argc, argv, true );
                typical.consumeArgs( &argc, argv, true );
            }
            //fprintf(stderr, "sleeping for 10secs after loop to find memloss");
            //sleep(10);
        }
        return 0;
    }catch( std::exception &e )
    {
        fprintf(stderr, "exception in main(): %s\n", e.what());
    }catch( ... )
    {
        ;//noop
    }
    return 1;
}

