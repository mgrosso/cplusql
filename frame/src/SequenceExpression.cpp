#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SequenceExpression.cpp
// Author:    mgrosso 
// Created:   Wed Nov  9 09:40:43 PST 2005 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "SequenceExpression.h"
#include "AppLog.h"
#include "Convert.h"

class SequenceExpressionPriv 
{
    private:
    friend class SequenceExpression ;
    long long       val_;
    bool            first_time_;
    char_ptr        last_reset_val_;
    char            raw_[33];
    ExpressionPtr   start_;
    ExpressionPtr   increment_;
    ExpressionPtr   reset_;
    SequenceExpressionPriv(
        ExpressionPtr   start,
        ExpressionPtr   increment,
        ExpressionPtr   reset
        ) : val_(0),first_time_(true),last_reset_val_(NULL),start_(start), increment_(increment), reset_(reset)
    {
        ;//noop
    };
};

SequenceExpression::SequenceExpression(
        ExpressionPtr   start,
        ExpressionPtr   increment,
        ExpressionPtr   reset
        )
{
    d_=new SequenceExpressionPriv ( start, increment, reset);
}

SequenceExpression::~SequenceExpression()
{
    delete d_;
}

void
SequenceExpression::notify() 
{
    //start + increment will be notified by parent so we just notify the
    //reset_ exp.  I know, I know too much about my parents implementation,
    //and yes it is disturbing.
    //
    d_->reset_->notify();
    d_->start_->notify();
    d_->increment_->notify();
    
    if(d_->first_time_)
    {
        d_->val_ = d_->start_->getLongLong();
        d_->last_reset_val_ = d_->reset_->getCharPtr();
        d_->first_time_=false;
    }
    else if( !strcmp( 
                d_->reset_->getRaw(), 
                d_->last_reset_val_.get()))
    {
        d_->val_ += d_->increment_->getLongLong();
    }
    else
    {
        d_->val_ = d_->start_->getLongLong();
        d_->last_reset_val_ = d_->reset_->getCharPtr();
    }
}

const char *
SequenceExpression::getRaw() 
{
    Convert::LongLongToString( d_->raw_, d_->val_ );
    return d_->raw_;
}

char_ptr
SequenceExpression::getCharPtr() const 
{
    return ptr_strdup_longlong( d_->val_);
}

long long int
SequenceExpression::getLongLong() const 
{
    return d_->val_;
}

long int
SequenceExpression::getLong() const
{
    return static_cast< long int > ( d_->val_ );
}

double
SequenceExpression::getDouble() const
{
    return static_cast< double > ( d_->val_ );
}

long double
SequenceExpression::getLongDouble() const
{
    return static_cast< long double > ( d_->val_ );
}

bool
SequenceExpression::getBool() const
{
    return static_cast< bool > ( d_->val_ );
}

bool
SequenceExpression::isNull() const
{
    return false;
}
