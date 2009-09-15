#ident "file_id $Id: CoalesceExpression.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CoalesceExpression.cpp
// Author:    mgrosso 
// Created:   Mon Jan 10 07:00:00 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: CoalesceExpression.cpp,v 1.1 2005/01/24 19:40:33 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CoalesceExpression.h"

class CoalesceExpressionPriv 
{
    private:
    ExpressionList          args_ ;
    const char *            raw_ ;
    friend class CoalesceExpression ;
    CoalesceExpressionPriv( ExpressionList &args )
    :args_(args),raw_("")
    {
        //noop
    };
};

CoalesceExpression::CoalesceExpression( ExpressionList &args )
{
    d_=new CoalesceExpressionPriv (args);
}

CoalesceExpression::~CoalesceExpression()
{
    delete d_;
}

const char *
CoalesceExpression::getRaw()
{
    return d_->raw_;
}

const char *
CoalesceExpression::getRaw() const
{
    return d_->raw_;
}

void
CoalesceExpression::notify()
{
    const char *gb ;
    for( size_t i=0; i< d_->args_->size(); ++i )
    {
        d_->args_->at(i)->notify();
        gb = d_->args_->at(i)->getRaw();
        if(! d_->args_->at(i)->isNull() && gb && *gb )
        {
            d_->raw_ = gb ;
            return ;
        }
    }
    d_->raw_="";
}

