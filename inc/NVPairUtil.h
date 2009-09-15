// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NVPairUtil.h
// Author:    mgrosso Matt Grosso
// Created:   Tue Mar  9 03:24:04 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: NVPairUtil.h,v 1.5 2004/06/01 19:27:54 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef NVPAIRUTIL_H
#define NVPAIRUTIL_H 1

#include "Config.h"
#include "serviceC.h"

class NVPairUtil
{
public:
    static ConfigPtr                convert( const Service::NVPairList & nvp );
	static Service::NVPairList *    convert( char *, char delim);
    static Service::NVPairList *    convert( const Config &conf );
    static void                     append(  Service::NVPairList *nvp, 
                                const char *key, const char *value );
	static void                     append( Service::NVPairList *nvp,
										const Config &cfg );
	static void                     append(Config &cfg, const Service::NVPairList &nvpl);
	static void                     append( Service::NVPairList *nvp, const char *key,
										long value);
};

#endif /* NVPAIRUTIL_H */
