// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ADSMapFactory.h
// Author:    djain
// Created:   Sun Jan 19 19:28:48 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: ADSMapFactory.h,v 1.1 2004/01/16 21:01:34 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef AbstractDelimitedSourceMAPFACTORY_H
#define AbstractDelimitedSourceMAPFACTORY_H 1

#include "Connection.h"
#include "AbstractDelimitedSourceMap.h"
#include "ptr.h"

class ADSMapFactory
{
	protected:
	ADSMapFactory();

    public:
	virtual ~ADSMapFactory();
	//really need var args
	virtual AbstractDelimitedSourceMapPtr createMap(ConnectionPtr & conn, const char * bucket) = 0;

    private:
};

typedef ptr<ADSMapFactory> ADSMapFactoryPtr;

#endif /* AbstractDelimitedSourceMAPFACTORY_H */
