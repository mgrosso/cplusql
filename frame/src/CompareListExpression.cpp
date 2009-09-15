#ident "file_id $Id$"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CompareListExpression.cpp
// Author:    mgrosso 
// Created:   Sat Jul 15 17:33:41 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CompareListExpression.h"
#include "CharExpression.h"

class CompareListExpressionPriv 
{
    private:
    friend class CompareListExpression ;
    ExpressionList          args_ ;
    ExpressionPtr           choice_;
    ExpressionPtr           null_;
    CompareListExpressionPriv(ExpressionList &args)
    :args_(args),null_(new CharExpression(""))
    {
        //noop
    };
};

CompareListExpression::CompareListExpression( ExpressionList &args )
{
    d_=new CompareListExpressionPriv (args);
}

CompareListExpression::~CompareListExpression()
{
    delete d_;
}

ExpressionPtr &
CompareListExpression::getProxy() const 
{
    return d_->choice_;
}

void
CompareListExpression::childNotify()
{
    ;//noop
}

ExpressionList &
CompareListExpression::getList()
{
    return d_->args_;
}

void
CompareListExpression::notify()
{
    size_t i,j,sz;
    sz=d_->args_->size();
    if(sz==0)
    {
        d_->choice_=d_->null_;
        return;
    }
    else if(sz==1)
    {
        d_->choice_=d_->args_->at(0);
        d_->choice_->notify();
        return;
    }
    for( i=0, j=1; j<sz; ++i, ++j )
    {
        ExpressionPtr lhs=d_->args_->at(i);
        ExpressionPtr rhs=d_->args_->at(j);
        lhs->notify();
        rhs->notify();
        d_->choice_=choose(lhs, rhs);
    }
}


