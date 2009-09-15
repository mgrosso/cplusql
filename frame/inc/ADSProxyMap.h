// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ADSProxyMap.h
// Author:    djain 
// Created:   Fri Dec 19 10:44:04 EST 2003 on lappy
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ADSProxyMap.h,v 1.3 2004/01/22 18:28:07 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ADSPROXYMAP_H
#define ADSPROXYMAP_H 1

#include "AbstractKeyValMap.h"
#include "AbstractDelimitedSourceMap.h"

class ADSProxyMapPriv;

class ADSProxyMap: public AbstractKeyValMap
{
	protected:
    virtual const char * get( ExpressionList &list );
    virtual const char * get( const char *key );

    public:
    ADSProxyMap(AbstractDelimitedSourceMapPtr & map, int index);
    virtual bool exists( ExpressionList &list );
    virtual bool exists( const char * key );
    virtual ~ADSProxyMap();
    virtual size_t size() const;

    private:
    ADSProxyMap();
    ADSProxyMapPriv *d_;
                                                                                                    
    //not implemented
    ADSProxyMap( const ADSProxyMap &rhs );
    ADSProxyMap &operator=( const ADSProxyMap &rhs );
};

#endif /* ADSPROXYMAP_H */
