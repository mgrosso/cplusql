#ident "file_id $Id: Config.cpp,v 1.25 2004/06/09 18:56:49 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Config.cpp
// Author:    Matt Grosso
// Created:   Thu Nov 12 00:01:25 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: Config.cpp,v 1.25 2004/06/09 18:56:49 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_LIMITS_H
#include "frame_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <string>

using namespace std;

#include "Convert.h"
#include "Config.h"
#include "ConfigPriv.h"
#include "Slurp.h"
#include "Parse.h"
#include "ConfigDump.h"
#include "ptr_strdup.h"

#define CONFIG_LINE_SIZE 16384

extern char ** environ;

void 
trimend( char *s ){
    if(!s || !(*s))
        return;
    int off=strlen(s);
    for( char *last=s+off-1; isspace(*last) && last!=s ;--last){
        *last='\0';
    }
};

void 
trimfront( char **s ){
    if(!s || !(*s) || !(**s))
        return;
    for(; (**s)&&isspace(**s);++(*s));
};

ConfigBadConversion::ConfigBadConversion() throw()
:BaseException("ConfigBadConversion")
{
    ;//noop
}

ConfigNoSuchKey::ConfigNoSuchKey() throw()
:BaseException("ConfigNoSuchKey")
{
    ;//noop
}

ConfigNoSuchKey::ConfigNoSuchKey(const char*c) throw()
:BaseException(c)
{
    ;//noop
}

ConfigNoSuchKey::ConfigNoSuchKey(string s)
:BaseException(s)
{
    ;//noop
}

Config::Config()
{
    d_=new ConfigPriv();
};

Config::Config( const char *filename )
{
    d_=new ConfigPriv();

	char_ptr s(Slurp::slurpFile( filename ));

	loadFile(s.get());
};

Config::Config( const Config &rhs )
{
    d_=new ConfigPriv();
    slurp( rhs, true );
};

Config &
Config::operator=( const Config &rhs )
{
    delete d_;
    d_=new ConfigPriv();
    slurp( rhs, true );
    return *this;
};

void
Config::clear()
{
	if (d_ != (ConfigPriv *) 0)
	{
		d_->cmap_.clear();
	}
};

void
Config::loadFile( char *contents )
{
    vector< char * > lines = Parse::parse( contents,"\n" );
    int sz=lines.size();
    for( int i = 0; i< sz; i++ ){
        char *line= lines[i];
        char *value=strpbrk( line, "=" );
        if((*line != '\0') && (value != NULL) && (*line != '#' )){
            *value++='\0' ;
            trimend(line);
            trimfront(&value);
            addString( line, value, true );
        }
    }
};

Config::~Config()
{
    delete d_ ;
};

void
Config::addString( const char *key, const char *value, bool replace )
{
    if( (!key)||(!value))
        return ;

    string k(key);
    string v(value);

    if(*value == '\'')
    {
        int length = strlen( value );
        char valuebuf[length + 1];
        char tmp[length + 1];
        memset(valuebuf, 0, length + 1);
        memset(tmp, 0, length + 1);
        strcpy(valuebuf,value );

        if(valuebuf[length - 1] == '\'')
        {
            valuebuf[length - 1] = '\0';
            --length;
        }
        int i = 0, j = 0;
        for(i = 1, j = 0; i < length; j++,i++)
        {
            if(valuebuf[i] == '\\' && valuebuf[i+1] == '\\')
            {
                tmp[j] = valuebuf[i+1];
                ++i;
            }
            if(valuebuf[i] == '\\' && valuebuf[i+1] == '\'')
            {
                tmp[j] = valuebuf[i+1];
                ++i;
            }
            else
            {
                tmp[j] = valuebuf[i];
            }
        }
        tmp[j] = '\0';
        v.assign(tmp);
    }

    pair<cmapi_t, bool> pback=d_->cmap_.insert( strp_t( k, v));
    if(! pback.second && replace ){
        d_->cmap_.erase(k); 
        d_->cmap_.insert( strp_t( k, v));
    }
};


void Config::addLong(const char *key, long long value, bool replace)
{
	char buf[128];

	(void) ::snprintf(buf, sizeof(buf), "%lld", value);

	addString(key, buf, replace);
}


const char *
Config::findString( const char *key ) const 
{
    string k(key);
    cmapi_t it=d_->cmap_.find( k );
    if(it==d_->cmap_.end())
        return NULL;
    return (*it).second.c_str();
};

const char *
Config::getString( const char *key ) const {
    const char *giveback=findString(key);
    if(giveback)
        return giveback;
    string err("ConfigNoSuchKey: ");
    err += key;
    ConfigNoSuchKey ck(err);
    throw ck;
    /* not reached */ return giveback;
};

const char *
Config::getString( const char *key, const char *dflt )
{
    const char *v = findString( key );
    if( ! v )
    {
        addString( key, dflt, true );
        return getString( key );
    }
    return v ;
};

int
Config::getInt( const char *key ) const 
{
    return Convert::convertToInt( getString(key));
};

int
Config::getInt( const char *key, const char *dflt )
{
    const char *v = findString( key );
    if( ! v )
    {
        addString( key, dflt, true );
        return Convert::convertToInt( dflt );
    }
    return Convert::convertToInt( v );
};

double
Config::getDouble( const char *key ) const 
{
    return Convert::convertToDouble( getString(key));
};

double
Config::getDouble( const char *key, const char *dflt ){
    const char *v = findString( key );
    if( ! v )
    {
        addString( key, dflt, true );
        return Convert::convertToDouble( dflt );
    }
    return Convert::convertToDouble( v );
}; 


bool
Config::getBool( const char *key ) const 
{
    return Convert::convertToBool( getString(key));
};

bool
Config::getBool( const char *key, const char *dflt )
{
    const char *v = findString( key );
    if( ! v )
    {
        addString( key, dflt, true );
        return Convert::convertToBool( dflt );
    }
    return Convert::convertToBool( v );
};

void 
Config::slurp( const ConfigPtr &rhs, bool override )
{
    cmapi_t itr;
    cmapi_t last=rhs->d_->cmap_.end();
    for( itr=rhs->d_->cmap_.begin(); itr!=last;++itr)
    {
        addString(
            (*itr).first.c_str(), 
            (*itr).second.c_str(), 
            override
        );
    }
};


void 
Config::slurp( const Config &rhs, bool override )
{
    cmapi_t itr;
    cmapi_t last=rhs.d_->cmap_.end();
    for( itr=rhs.d_->cmap_.begin(); itr!=last;++itr)
    {
        addString(
            (*itr).first.c_str(), 
            (*itr).second.c_str(), 
            override
        );
    }
	//ConfigDump::write(rhs, 2);
};

void 
Config::slurp( const char *filename, bool override )
{
    Config rhs(filename);
    slurp(rhs,override);
};

void 
Config::loadEnv( bool override )
{
    int i;
    char *nextpair;
    for( 
        i=0, nextpair=environ[i]; 
        nextpair && *nextpair ;  
        nextpair=environ[++i] )
    {
        char line[CONFIG_LINE_SIZE] ;
        strncpy( line, nextpair, CONFIG_LINE_SIZE );
        char *value=strpbrk( line, "=" );
        if((*line != '\0') && (value != NULL) ){
            *value++='\0' ;
            trimend(line);
            trimfront(&value);
            addString( line, value, override );
        }
    }
}

void 
Config::consumeArgs( int *argc, char **argv, bool override )
{
    loadArgs( *argc, argv, override );
    if(*argc<2) 
    {
        return;
    }
    //
    //heads up!
    //
    //the following loop may be a bit tricky to follow, partly
    //because we advance through the loop by incrementing i only
    //if we are not removing the option from argv, otherwise we
    //advance through the loop by decrementing *argc.
    //
    for( int i=1 ; i<*argc ; )
    {
        if(
            (! strncmp( argv[i], "--", 2 ))&&
            (2<strlen(argv[i]))
        )
        {
            if( i+1 < *argc)
            {
                //shift argv "left"
                int j;
                for( j=i; j+1 <*argc; ++j )
                {
                    argv[j]=argv[j+1];
                }
                argv[j]=NULL;//j == argc-1  at this point.
                -- *argc ;
            }else{
                //last one. 
                argv[i]=NULL;
                -- *argc ;
                break;
            }
        }else{
            ++i;
        }
    }
}

void 
Config::loadArgs( int argc, char **argv, bool override )
{
    if(argc<2) return;
    int i;
    char *arg;
    for( i=1, arg=argv[i] ; i<argc ; ++i, arg=argv[i] )
    {
        if( 
            (! strncmp( arg, "--", 2 ))&&
            (strchr(arg,'='))
        )
        {
            char line[CONFIG_LINE_SIZE] ;
            memset(line,'\0',CONFIG_LINE_SIZE );
            strncpy( line, arg+2, CONFIG_LINE_SIZE );
            char *value=strpbrk( line, "=" );
            if((*line != '\0') && (value != NULL) ){
                *value++='\0' ;
                trimend(line);
                trimfront(&value);
                addString( line, value, override );
            }
        }else if(
            (! strncmp( arg, "--", 2 ))&&
            (2<strlen(arg))
        )
        {
            ++arg; 
            ++arg;
            addString( arg, "true", true );
        }
    }
}

void
Config::slurpString( const char *configstr, bool override )
{
    char_ptr cp = ptr_strdup_wnull( configstr );
    if( override )
    {
        loadFile( cp.get());
    }else
    {
        Config c;
        c.loadFile( cp.get());
        slurp( c, false );
    }
}


char_ptr_vec *
Config::getKeys() const
{
	cmap_t::size_type sz = d_->cmap_.size();
	cmap_t::size_type i = 0;
	char_ptr_vec *retVal = new char_ptr_vec(sz);

	for (constcmapi_t itor = d_->cmap_.begin(); itor != d_->cmap_.end(); ++itor)
	{
		if (i < sz)
		{
			(*retVal)[i++] = ptr_strdup(itor->first.c_str());
		}
	}

	return retVal;
}
