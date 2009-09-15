#ident "file_id $Id: NVPairUtil.cpp,v 1.7 2004/08/03 23:33:04 sjackson Exp $ "
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NVPairUtil.cpp
// Author:    mgrosso Matt Grosso
// Created:   Tue Mar  9 03:24:04 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: NVPairUtil.cpp,v 1.7 2004/08/03 23:33:04 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "NVPairUtil.h"
#include "ConfigDump.h"
#include "BaseException.h"

ConfigPtr        
NVPairUtil::convert( const Service::NVPairList & nvpl )
{
	Config *c = new Config();

	append(*c, nvpl);

    ConfigPtr retVal = c;

    return retVal;
}

Service::NVPairList *     
NVPairUtil::convert( const Config &conf )
{
    Service::NVPairList * snvpl = new Service::NVPairList();

	append(snvpl, conf);

	return snvpl;
}


void
NVPairUtil::append(Service::NVPairList *snvpl, const Config &conf)
{
    pConfigDumpT dump = ConfigDump::dump( conf );
    snvpl->length( dump->size());
    ConfigDumpT::iterator i;
    ConfigDumpT::iterator e;
    unsigned int j ;
    for( i=dump->begin(), e=dump->end(), j=0; i!=e; ++i, ++j )
    {
        Service::NVPair nvp;
        nvp.name = i->first.c_str();
        nvp.value = i->second.c_str();
        (*snvpl)[j]=nvp;
    }
}

void
NVPairUtil::append(Config &cfg, const Service::NVPairList &nvpl)
{
    unsigned long  sz = nvpl.length();
    for( unsigned long i=0; i< sz; ++i )
    {
        const char *name = nvpl[i].name ;
        const char * value = nvpl[i].value;
        cfg.addString( name, value, 1 );
    }
}


Service::NVPairList *
NVPairUtil::convert(char *s, char delim)
{
	Service::NVPairList *retVal = new Service::NVPairList();
	int cnt = 1;

	for (char *b = s; *b != '\0'; b++)
	{
		if (*b == delim)
		{
			cnt++;
		}
	}

	retVal->length(cnt);
	cnt = 0;

	while (s != (const char *) 0)
	{
		char *f = strchr(s, delim);

		if (f != (char *) 0)
		{
			*f = '\0';
		}

		if (strlen(s) > 0)
		{
			const char sep = '=';
			Service::NVPair nvp;
			char *eq = strchr(s, sep);
			char *val;

			if (eq != (char *) 0)
			{
				*eq = '\0';
				val = strdup_wnew(eq + 1);
			}
			else
			{
				val = (char *) 0;
			}

			nvp.name = strdup_wnew(s);
			nvp.value = val;
			(*retVal)[cnt++] = nvp;

			if (eq != (char *) 0)
			{
				*eq = sep;
			}
		}

		if (f != (char *) 0)
		{
			*f = delim;
			s = ++f;
		}
		else
		{
			s = (char *) 0;
		}
	}

	retVal->length(cnt);

	return retVal;
}


void             
NVPairUtil::append( Service::NVPairList *nvp, const char *name, const char *value )
{
    if(!nvp || ! name || ! value )
    {
        PANICV( "NVPairUtil::append() Null argument: %lu  %lu  %lu ",
            reinterpret_cast<unsigned long >( nvp ),          
            reinterpret_cast<unsigned long >( name ),          
            reinterpret_cast<unsigned long >( value )
        );
    }
    unsigned int last = nvp->length()+1 ;
    nvp->length( last+1 );

    (*nvp)[last].name = name ;
    (*nvp)[last].value = value ;
}


void
NVPairUtil::append(Service::NVPairList *nvp, const char *nm, long val)
{
	char buf[128];

	(void) snprintf(buf, sizeof(buf), "%ld", val);
	NVPairUtil::append(nvp, nm, buf);
}

