#ident "file_id $Id: ColumnExpression.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ColumnExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:37:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: ColumnExpression.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ColumnExpression.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"

class ColumnExpressionPriv
{
    friend class ColumnExpression ;
private:
    ColumnExpressionPriv( const DelimitedSourcePtr &rp )
    :rp_(rp)
    {
        ;//noop
    };
    ColumnExpressionPriv( const RelationPtr &rp )
    :rp_(rp)
    {
        ;//noop
    };

    DelimitedSourcePtr rp_;
    size_t column_;
};

ColumnExpression::ColumnExpression( const DelimitedSourcePtr &rp, size_t column )
{
    d_=new ColumnExpressionPriv(rp);//
    d_->column_=column ;
}

ColumnExpression::ColumnExpression( RelationPtr &rp, size_t column )
{
    d_=new ColumnExpressionPriv(rp);//
    d_->column_=column ;
}

ColumnExpression::~ColumnExpression()
{
    delete d_;
}


bool                
ColumnExpression::isNull() const
{
    return Convert::isNull( d_->rp_->getRaw( d_->column_));
}

const char *
ColumnExpression::getRaw()
{
    return d_->rp_->getRaw( d_->column_);
}

char_ptr
ColumnExpression::getCharPtr() const
{
    return d_->rp_->getCharPtr( d_->column_);
}

bool                
ColumnExpression::getBool() const
{
    return d_->rp_->getBool( d_->column_);
}

long int
ColumnExpression::getLong() const
{
    return d_->rp_->getLong( d_->column_);
}

double              
ColumnExpression::getDouble() const
{
    return d_->rp_->getDouble( d_->column_);
}

long long
ColumnExpression::getLongLong() const
{
    return d_->rp_->getLongLong( d_->column_);
}

long double
ColumnExpression::getLongDouble() const
{
    return d_->rp_->getLongDouble( d_->column_);
}


