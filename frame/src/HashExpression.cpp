#ident "file_id $Id: HashExpression.cpp,v 1.9 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Thu Feb  6 07:08:19 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashExpression.cpp,v 1.9 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "BaseException.h"
#include "HashExpression.h"
#include "AbstractHashAlg.h"
#include "HashAlgFactory.h"

class HashExpressionPriv
{
    friend class HashExpression;
    private:
    HashExpressionPriv( ptr< AbstractHashAlg > hash, const ExpressionPtr &e )
    :hash_(hash),e_(e)
    {
        ;//noop
    };
    ptr< AbstractHashAlg > hash_ ;
    ExpressionPtr e_;
    char_ptr raw_;
    long long val_;
};

HashExpression::HashExpression( const ExpressionPtr &dsp, unsigned int buckets )
{
    d_=new HashExpressionPriv( HashAlgFactory::getBasicHashPtr( buckets ), dsp );
    if(!d_)
    {
        THROW_NOMEM;
    }
}

HashExpression::HashExpression()
{
    ;//noop
}

HashExpression::~HashExpression()
{
    ;//noop
}

bool                
HashExpression::isNull() const 
{
    return d_->e_->isNull();
}

void 
HashExpression::notify(void) 
{
    d_->e_->notify();
    d_->val_=d_->hash_->hash( d_->e_->getRaw() );
    d_->raw_=ptr_strdup_longlong( d_->val_ );
};

const char *        
HashExpression::getRaw() 
{
    return d_->raw_.get();
};

char_ptr            
HashExpression::getCharPtr() const 
{
    return d_->raw_;
}

bool                
HashExpression::getBool() const 
{
    return d_->val_ ? true : false ;
}

long int
HashExpression::getLong() const 
{
    return static_cast< long int >( d_->val_);
}

double              
HashExpression::getDouble() const 
{
    return static_cast< double >( d_->val_);
}

long long
HashExpression::getLongLong() const 
{
    return d_->val_;
}

long double
HashExpression::getLongDouble() const 
{
    return static_cast< long double >( d_->val_);
}

