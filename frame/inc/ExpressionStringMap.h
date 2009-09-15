// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExpressionStringMap.h
// Author:    mgrosso 
// Created:   Thu Mar 27 18:09:02 EST 2003 on dhcp-172-18-102-121.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ExpressionStringMap.h,v 1.6 2004/01/22 18:28:07 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef EXPRESSIONSTRINGMAP
#define EXPRESSIONSTRINGMAP 1

#include "AbstractKeyValSetMap.h"

class ExpressionStringMapPriv;

class ExpressionStringMap: public AbstractKeyValSetMap
{
	protected:
    virtual const char * get( ExpressionList &list );
    virtual const char * get( const char *key );

    public:
    ExpressionStringMap();
    virtual ~ExpressionStringMap();
    virtual void set( const char *key, const char *val );
    virtual void set( ExpressionPtr &key, ExpressionPtr &val );
    virtual void clear();
    virtual bool exists( ExpressionList &list );
    virtual bool exists( const char * key );
    virtual size_t size() const;

    private:
    ExpressionStringMapPriv *d_;
    
    //not implemented
    ExpressionStringMap( const ExpressionStringMap &rhs );
    ExpressionStringMap &operator=( const ExpressionStringMap &rhs );
};

#endif /* EXPRESSIONSTRINGMAP */
