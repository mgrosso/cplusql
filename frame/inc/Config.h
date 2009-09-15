// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Config.h
// Author:    Matt Grosso
// Created:   Wed Nov 11 22:50:10 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Config.h,v 1.12 2004/06/09 18:56:49 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CONFIG_H
#define CONFIG_H 1

#include "BaseException.h"
#include "ptr.h"
#include "ptr_strdup.h"

class ConfigBadConversion: public BaseException 
{
    public:
    ConfigBadConversion() throw();
};

class ConfigNoSuchKey: public BaseException 
{
    public:
    ConfigNoSuchKey() throw();
    ConfigNoSuchKey(const char*) throw();
    ConfigNoSuchKey(string) ;
};

class Config;
class ConfigPriv;
class ConfigDump ;

typedef ptr< Config > ConfigPtr ;

class Config {
    friend class ConfigDump ;
public:
    Config();
    Config( const char *filename );
    Config( const Config &rhs );
    Config & operator = ( const Config &rhs );
    virtual ~Config();

	void clear();

    //------------------------------------------------------------
    // slurp reads in a file or Config and adds its values to self.
    // override, if true, will cause the new values to override
    // old ones. also see addString for more information.
    // slurpString takes a char * which should be in the same format
    // as a file would be.
    //------------------------------------------------------------
    virtual void slurp( const char * filename, bool override );
    virtual void slurp( const Config &rhs, bool override );
    virtual void slurp( const ConfigPtr &rhs, bool override );
    virtual void slurpString( const char *configstr, bool override );

    //------------------------------------------------------------
    // unix environmental variables become config keys
    //------------------------------------------------------------
    void loadEnv( bool override = true );

    //------------------------------------------------------------
    // looks for and loads args of format --key=value
    // does not alter or consume any args though.
    //------------------------------------------------------------
    void loadArgs( int argc, char ** argv, bool override=true );
    void consumeArgs( int *argc, char ** argv, bool override=true );

    //------------------------------------------------------------
    // write() writes the config out in its own format. to be 
    // implemented just as soon as I have a real use for it.
    //------------------------------------------------------------
    //void write( const char *filename );
    //void write( int fd );

    //findString returns NULL if key is not in self.
    const char *    findString( const char *key )const;

	// Caller is responsible for deleting the object and contents.
	//
	char_ptr_vec * getKeys() const;


    //------------------------------------------------------------
    //getString, getInt, and getDouble get the values corresponding
    // to their names. If no such key exists, ConfigNoSuchKey is thrown.
    // If the internal string cannot be converted into an int or double, 
    // ConfigBadConversion is thrown.
    //------------------------------------------------------------
    virtual
    const char *    getString( const char *key )const;
    int             getInt( const char *key )const;
    double          getDouble( const char *key )const;
    bool            getBool( const char *key )const;

    const char *    getString( const char *key, const char *dflt );
    int             getInt( const char *key, const char *dflt );
    double          getDouble( const char *key, const char *dflt );
    bool            getBool( const char *key, const char *dflt );


    //------------------------------------------------------------
    // addString adds the key and value to the Config's map. If 
    // replace is true, then value will replace any existing value
    // if the key allready exists.
    // 
    // certain substitutions will be made in 'value' iff value begins
    // with the character '
    //  - first and last ' are removed.
    //  - \\ is replaced with the \ character
    //  - \' is replace with '
    //  - ' by itself in the middle of the string is passed through.
    //  - therefore \\' will result in \'
    //  
    // this method, and thus these substitutinos, are used by slurp(), 
    // as well as loadEnv() and loadArgs()
    //  
    //------------------------------------------------------------
    void            addString( 
                        const char *key, 
                        const char *value,
                        bool replace 
                    );
    void            addLong( 
                        const char *key, 
                        long long value,
                        bool replace 
                    );

private:
    ConfigPriv *d_;
    void loadFile( char *contents );
};

#endif /* CONFIG_H */


