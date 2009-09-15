// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SlurpedSourcePriv.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Sep 15 13:29:00 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: SlurpedSourcePriv.h,v 1.1 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SLURPEDSOURCEPRIV_H
#define SLURPEDSOURCEPRIV_H 1

#include <vector>

class SlurpedSourcePriv
{
    public:
    friend class SlurpedSource ;
    char_ptr cp_;
    ptr< vector< char * > > pv_;
    vector< char * >::iterator vi_;
    vector< char * >::iterator ve_;
    int line_;
};

#endif /* SLURPEDSOURCEPRIV_H */
