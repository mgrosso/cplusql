#ident "file_id $Id: RangedUnaryArithmeticExpression.cpp,v 1.1 2004/09/21 16:30:57 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RangedUnaryArithmeticExpression.cpp
// Author:    mgrosso 
// Created:   Sat Sep  4 14:40:10 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: RangedUnaryArithmeticExpression.cpp,v 1.1 2004/09/21 16:30:57 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RangedUnaryArithmeticExpression.h"
#include "RangedUnaryArithmeticExpressionPriv.h"
#include "Convert.h"
#include "ptr_strdup.h"

RangedUnaryArithmeticExpression::RangedUnaryArithmeticExpression( 
    ExpressionPtr &operand, ExpressionPtr &ifedom )
{
    d_=new RangedUnaryArithmeticExpressionPriv ( operand, ifedom );
}

RangedUnaryArithmeticExpression::~RangedUnaryArithmeticExpression()
{
    delete d_;
}

long double
RangedUnaryArithmeticExpression::edom()
{
    d_->ifedom_->notify();
    return d_->ifedom_->getLongDouble();
}

bool                
RangedUnaryArithmeticExpression::isNull() const 
{
    return d_->operand_->isNull();
}

void 
RangedUnaryArithmeticExpression::notify(void) 
{
    d_->operand_->notify();
    d_->value_ = getLongDouble( d_->operand_->getLongDouble());
};

const char *        
RangedUnaryArithmeticExpression::getRaw() 
{
    Convert::LongDoubleToString( d_->raw_, d_->value_ );
    return d_->raw_;
}

char_ptr            
RangedUnaryArithmeticExpression::getCharPtr() const 
{
    Convert::LongDoubleToString( (char *)d_->raw_, d_->value_ );
    return ptr_strdup( d_->raw_ );
}

bool                
RangedUnaryArithmeticExpression::getBool() const 
{
    return d_->value_ ? true : false ;
}

long int
RangedUnaryArithmeticExpression::getLong() const 
{
    return static_cast< long int >( d_->value_);
}

double              
RangedUnaryArithmeticExpression::getDouble() const 
{
    return static_cast< double >( d_->value_);
}

long long
RangedUnaryArithmeticExpression::getLongLong() const 
{
    return static_cast< long long >( d_->value_);
}

long double
RangedUnaryArithmeticExpression::getLongDouble() const 
{
    return d_->value_;
}

