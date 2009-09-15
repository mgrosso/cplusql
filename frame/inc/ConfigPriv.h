// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ConfigPriv.h
// Author:    Matt Grosso
// Created:   Wed Nov 11 23:43:24 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: ConfigPriv.h,v 1.2 2001/01/28 19:08:54 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CONFIGPRIV_H
#define CONFIGPRIV_H 1

#include <map>
#include <string>

#include "stringlt.h"

typedef map< string, string, stringlt > cmap_t ;
typedef map< string, string, stringlt >::iterator cmapi_t ;
typedef map< string, string, stringlt >::const_iterator constcmapi_t ;
typedef pair< string, string > strp_t ;
class Config ;
class ConfigDump ;

class ConfigPriv {
private:
    friend class Config ;
    friend class ConfigDump ;
    cmap_t cmap_ ;
};
#endif /* CONFIGPRIV_H */
