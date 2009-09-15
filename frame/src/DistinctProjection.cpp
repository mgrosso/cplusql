#ident "file_id $Id: DistinctProjection.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DistinctProjection.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Dec 15 20:58:58 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DistinctProjection.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ptr.h"
#include "hashFuncs.h"
#include "DistinctProjection.h"

DistinctProjection::DistinctProjection( 
    ExpressionPtr &groupby_col, 
    AbstractProjectionPtr &next 
)
:nextp_(next),exp_(groupby_col),started_(false)
{
    ;//noop
}

DistinctProjection::~DistinctProjection()
{
    ;//noop
}

void
DistinctProjection::loadRow()
{
    distinct_map_t::iterator i;
    distinct_map_t::iterator e;
    char_ptr v = exp_.getCharPtr();
    i = distinct_.find( exp_.getCharPtr())
    if( i!=e )
    {
        return;
    }
    AbstractProjectionPtr n = next_.copy();
    exp_.insert( pair< char_ptr, AbstractProjectionPtr > ( v, next_.copy()));
    next_=n.get();
}

ptr< AbstractProjection >    
DistinctProjection::copy()
{
    return ptr< AbstractionProjection > ( 
        new AbstractionProjection( nextp_, exp_ )
    );
}

bool                
DistinctProjection::isNull()const
{
    return Convert::isNull( (*current_row_).get() );
}

const char *        
DistinctProjection::getRaw()
{
    return (*current_row_).get();
}

char_ptr            
DistinctProjection::getCharPtr()const
{
    return (*current_row_);
}

bool                
DistinctProjection::getBool()const
{
    return Convert::convertToBool( getRaw());
}

long int            
DistinctProjection::getLong()const
{
    return Convert::convertToLong( getRaw());
    ;
}

double              
DistinctProjection::getDouble()const
{
    return Convert::convertToDouble( getRaw());
}

long long           
DistinctProjection::getLongLong()const
{
    return Convert::convertToLongLong( getRaw());
}


