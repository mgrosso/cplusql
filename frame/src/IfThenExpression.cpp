#ident "file_id $Id: IfThenExpression.cpp,v 1.7 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IfThenExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb 10 08:58:59 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: IfThenExpression.cpp,v 1.7 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "IfThenExpression.h"
#include "BaseException.h"
#include "Convert.h"

class IfThenExpressionPriv
{
    private:
    friend class IfThenExpression ;
    ExpressionPtr condition_;
    ExpressionPtr texp_;
    ExpressionPtr fexp_;
    bool cur_;
    IfThenExpressionPriv( ExpressionPtr &c, ExpressionPtr &t, ExpressionPtr &f)
    :condition_(c),texp_(t),fexp_(f),cur_(false)
    {
        ;//noop
    };
};


IfThenExpression::IfThenExpression( 
    ExpressionPtr &c, ExpressionPtr &t, ExpressionPtr &f
)
{
    d_=new IfThenExpressionPriv( c,t,f );
    if(!d_)
    {
        THROW_NOMEM;
    }
}

IfThenExpression::~IfThenExpression()
{
    delete d_;
}


ExpressionPtr &
IfThenExpression::getProxy() const 
{
    if( d_->cur_)
    {
        return d_->texp_ ;
    }
    return d_->fexp_ ;
}

void
IfThenExpression::notify() 
{
    d_->condition_->notify();
    if(d_->condition_->getBool())
    {
        d_->texp_->notify();
        d_->cur_= true;
    }else{
        d_->fexp_->notify();
        d_->cur_= false;
    }
}

