// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestHashFiles.h
// Author:    mgrosso Matthew Grosso
// Created:   Tue Oct 29 04:08:04 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DestHashFiles.h,v 1.8 2004/12/31 20:57:28 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DESTHASHFILES_H
#define DESTHASHFILES_H 1

#include "Dest.h"
#include "BaseException.h"
#include "ptr.h"
#include "ptr_strdup.h"
#include "Config.h"
#include "HashHelper.h"
#include "AbstractDestFactory.h"

class DestHashFilesPriv ;

class DestHashFiles : public Dest
{
    public:
    DestHashFiles(
        ptr< HashHelper > hashhelper,
        ptr< AbstractDestFactory > destFactory,
        const char *filename_pattern, 
        ConfigPtr & cfg
    );
    virtual ~DestHashFiles();
    virtual void put(const char *s );
    virtual void put(const char *s, int slen );
    virtual void flush(void);

    private:
    pDest getDest();
    pDest getDest_(char_ptr & bucket);

    DestHashFilesPriv *d_;

    //not implemented
    DestHashFiles();
    DestHashFiles(const DestHashFiles &rhs);
    DestHashFiles &operator= (const DestHashFiles &rhs);
};

#endif /* DESTHASHFILES_H */
