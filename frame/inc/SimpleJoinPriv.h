// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SimpleJoinPriv.h
// Author:    mgrosso 
// Created:   Sun Jul  7 18:20:09 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: SimpleJoinPriv.h,v 1.3 2003/01/22 05:45:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SIMPLEJOINPRIV_H
#define SIMPLEJOINPRIV_H 1

#include "ptr.h"
#include "ptr_strdup.h"
#include "NumberTypes.h"
#include <map>

class SimpleJoin;

typedef map< out_key_t, char_ptr >  kcpmap_t ;
typedef ptr< kcpmap_t >             pkcpmap_t ;


class SimpleJoinPriv
{
    public:
    friend class SimpleJoin;

    private:
    pkcpmap_t j_;

};

#endif /* SIMPLEJOINPRIV_H */
