#ident "file_id $Id: ColumnElementExpression.cpp,v 1.8 2005/01/24 19:29:04 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ColumnElementExpression.cpp
// Author:    djain Deepak Jain
// Created:   Wed Feb  5 14:00:38 PST 2003 on sf-devdw002.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ColumnElementExpression.cpp,v 1.8 2005/01/24 19:29:04 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ColumnElementExpression.h"
#include "ParsedColumns.h"
#include "Convert.h"
#include "BaseException.h"

class ColumnElementExpressionPriv
{
    friend class ColumnElementExpression ;
    private:
        ParsedColumnsPtr col_;
        unsigned int field_index_;
};

ColumnElementExpression::ColumnElementExpression( const ParsedColumnsPtr & col, unsigned int field_index)
{
    d_=new ColumnElementExpressionPriv;
    d_->col_=col;
    d_->field_index_=field_index;
}

ColumnElementExpression::~ColumnElementExpression()
{
	delete d_;
}

void                
ColumnElementExpression::notify() 
{
    //the ParsedColumns object has allready been notify()ed by the relation 
    //to which it is subscribed.  Doing that here would notify once per column!
    //d_->col_->notify();
}

bool                
ColumnElementExpression::isNull() const
{
    return Convert::isNull( d_->col_->getField( d_->field_index_ ) );
}

const char *
ColumnElementExpression::getRaw()
{
    return d_->col_->getField( d_->field_index_);
}

char_ptr
ColumnElementExpression::getCharPtr() const
{
    return ptr_strdup( d_->col_->getField( d_->field_index_) );
}

bool                
ColumnElementExpression::getBool() const
{
    return Convert::convertToBool( d_->col_->getField( d_->field_index_ ) );
}

long int
ColumnElementExpression::getLong() const
{
    return Convert::convertToLong( d_->col_->getField( d_->field_index_ ) );
}

double              
ColumnElementExpression::getDouble() const
{
    return Convert::convertToDouble( d_->col_->getField( d_->field_index_ ) );
}

long long
ColumnElementExpression::getLongLong() const
{
    return Convert::convertToLongLong( d_->col_->getField( d_->field_index_ ) );
}

long double
ColumnElementExpression::getLongDouble() const
{
    return Convert::convertToLongDouble( d_->col_->getField( d_->field_index_ ) );
}
