// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractKeyValSetMap.h
// Author:    djain 
// Created:   Thu Mar 27 18:09:02 EST 2003 on dhcp-172-18-102-121.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: AbstractKeyValSetMap.h,v 1.1 2004/01/16 21:01:34 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACTKEYVALSETMAP_H
#define ABSTRACTKEYVALSETMAP_H 1

#include "AbstractKeyValMap.h"

class AbstractKeyValSetMap: public AbstractKeyValMap
{
	protected:
    AbstractKeyValSetMap();

    public:
    virtual ~AbstractKeyValSetMap();
    virtual void set( const char *key, const char *val ) = 0;
    virtual void set( ExpressionPtr &key, ExpressionPtr &val ) = 0;

    private:
    //not implemented
    AbstractKeyValSetMap( const AbstractKeyValSetMap &rhs );
    AbstractKeyValSetMap &operator=( const AbstractKeyValSetMap &rhs );
};

typedef ptr< AbstractKeyValSetMap > AbstractKeyValSetMapPtr ;
#endif /* ABSTRACTKEYVALSETMAP_H */
