#ident "file_id $Id: SingleRowDelimitedSource.cpp,v 1.1 2004/01/16 21:22:07 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SingleRowDelimitedSource.cpp
// Author:    djain 
// Created:   Sun Apr 27 18:15:05 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: SingleRowDelimitedSource.cpp,v 1.1 2004/01/16 21:22:07 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SingleRowDelimitedSource.h"
#include "Convert.h"

class SingleRowDelimitedSourcePriv 
{
    private:
    friend class SingleRowDelimitedSource ;

    SingleRowDelimitedSourcePriv(
		DelimitedSourcePtr & ds
	)
	{
		num_elems = ds->getNumColumns();
		elems = new char*[num_elems];
		for(int i = 0; i < num_elems; ++i)
			elems[i] = strdup_wnew(ds->getRaw(i));
	}

	virtual ~SingleRowDelimitedSourcePriv()
	{
		for(int i = 0; i < num_elems; ++i)
			delete elems[i];

		delete [] elems;
	}

	char ** elems;
	int num_elems;
};

SingleRowDelimitedSource::SingleRowDelimitedSource( 
		DelimitedSourcePtr & ds
)
{
    d_=new SingleRowDelimitedSourcePriv ( ds );
}

SingleRowDelimitedSource::~SingleRowDelimitedSource()
{
    delete d_;
}

size_t
SingleRowDelimitedSource::getNumColumns() const
{
	return d_->num_elems;
}

int
SingleRowDelimitedSource::lineNum()
{
	return 1;
}

const char *        
SingleRowDelimitedSource::getRaw( size_t index ) 
{
    return d_->elems[index];
}

const char *        
SingleRowDelimitedSource::getRaw( size_t index ) const
{
    return d_->elems[index];
}

char_ptr            
SingleRowDelimitedSource::getCharPtr( size_t index ) const 
{
    return ptr_strdup(getRaw(index)); 
}

bool
SingleRowDelimitedSource::getBool( size_t index ) const
{
    return Convert::convertToBool( getRaw(index ));
}

long int
SingleRowDelimitedSource::getLong( size_t index ) const
{
    return Convert::convertToLong( getRaw(index ));
}

double
SingleRowDelimitedSource::getDouble( size_t index ) const
{
    return Convert::convertToDouble( getRaw(index ));
}

long long
SingleRowDelimitedSource::getLongLong( size_t index ) const
{
    return Convert::convertToLongLong( getRaw( index ));
}

long double
SingleRowDelimitedSource::getLongDouble( size_t index ) const
{
    return Convert::convertToLongDouble( getRaw( index ));
}
