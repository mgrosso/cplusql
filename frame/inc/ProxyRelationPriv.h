// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ProxyRelationPriv.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Dec  7 10:17:39 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ProxyRelationPriv.h,v 1.1 2003/01/22 05:45:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef PROXYRELATIONPRIV_H
#define PROXYRELATIONPRIV_H 1
#include "ProxyRelation.h"

class ProxyRelationPriv
{
    public:
    RelationMetaPtr         meta_;
    ExpressionList          all_;
};

#endif /* PROXYRELATIONPRIV_H */
