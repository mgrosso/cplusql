// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AppConfig.h
// Author:    Matt Grosso
// Created:   Sun Dec 27 14:37:06 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: AppConfig.h,v 1.3 2004/08/04 23:02:37 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef APPCONFIG_H
#define APPCONFIG_H 1

#include "Config.h"

#define APPCONFIG_FILES_KEY "APPCONFIG_FILES"

class AppConfig : public Config 
{
public:
    static AppConfig *      instance();
	static void             shutdown();
    virtual void            init( int argc, char **argv );
private:
    static AppConfig *instance_;
};

#endif /* APPCONFIG_H */
