// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CopyCacheDSMap.h
// Author:    mgrosso 
// Created:   Thu Mar 27 18:09:02 EST 2003 on dhcp-172-18-102-121.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CopyCacheDSMap.h,v 1.3 2004/09/21 00:13:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef COPYCACHEMAP
#define COPYCACHEMAP 1

#include "AbstractDelimitedSourceMap.h"

class CopyCacheDSMapPriv;

class CopyCacheDSMap: public AbstractDelimitedSourceMap
{
	protected:
    virtual DelimitedSourcePtr get_( const char * key );
    virtual DelimitedSourcePtr get_( ExpressionList & exp );
    virtual bool exists_( const char * key );
    virtual bool exists_( ExpressionList & list );
    virtual size_t size_();

    public:
    CopyCacheDSMap(AbstractDelimitedSourceMapPtr & map);
    virtual ~CopyCacheDSMap();

    //extensions
    virtual const char *    getRelationName()const;
    virtual char_ptr        getRelationHeader()const;
    virtual size_t          getNumColumns()const;
    virtual const char *    getColumnName( size_t index )const;
    virtual size_t          getColumnIndex( const char *col_name )const;

	virtual void			setCacheSize( int size );
	virtual int				getCacheSize() const;

    private:
    CopyCacheDSMapPriv *d_;
	void clearStale();
    
    //not implemented
    CopyCacheDSMap( const CopyCacheDSMap &rhs );
    CopyCacheDSMap &operator=( const CopyCacheDSMap &rhs );
};

#endif /* COPYCACHEMAP */
