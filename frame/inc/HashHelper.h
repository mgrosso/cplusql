// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashHelper.h
// Author:    mgrosso Matthew Grosso
// Created:   Tue Oct 29 04:08:04 EST 2002 on circe
// Project:
// Purpose:
//
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
//
// $Id: HashHelper.h,v 1.1 2004/01/16 21:01:33 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef HASHHELPER_H
#define HASHHELPER_H 1
 
#include "ptr_strdup.h"
 
class HashHelper
{
    public:
    virtual char_ptr getHash() const =0 ;
};
 
typedef ptr<HashHelper> HashHelperPtr;
 
#endif /* HASHHELPER_H */

