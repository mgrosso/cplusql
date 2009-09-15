// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractKeyValMap.h
// Author:    mgrosso 
// Created:   Thu Mar 27 18:09:02 EST 2003 on dhcp-172-18-102-121.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: AbstractKeyValMap.h,v 1.3 2004/01/22 18:28:07 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACTKEYVALMAP_H
#define ABSTRACTKEYVALMAP_H 1

#include "Expression.h"

class AbstractKeyValMapPriv;

class AbstractKeyValMap
{
	protected:
    virtual const char * get( ExpressionList &list ) = 0;
    virtual const char * get( const char *key ) = 0;

    public:
    AbstractKeyValMap(bool leak_on_delete = false);
    virtual ~AbstractKeyValMap();
    virtual const char * get( ExpressionList &list, const char * dflt );
    virtual const char * get( ExpressionList &list, ExpressionPtr &deflt );
    virtual const char * get( ExpressionPtr &key, const char * dflt );
    virtual const char * get( ExpressionPtr &key, ExpressionPtr &deflt );
    virtual bool exists( ExpressionList &list ) = 0;
    virtual bool exists( ExpressionPtr &key );
    virtual bool exists( const char * key ) = 0;

    virtual size_t size() const = 0;
    virtual void clear();
    virtual void setLeakOnDelete( bool leak );
    virtual bool getLeakOnDelete();

    private:
	AbstractKeyValMapPriv *d_;

    //not implemented
    AbstractKeyValMap( const AbstractKeyValMap &rhs );
    AbstractKeyValMap &operator=( const AbstractKeyValMap &rhs );
};

typedef ptr< AbstractKeyValMap > AbstractKeyValMapPtr ;

#endif /* ABSTRACTKEYVALMAP_H */
