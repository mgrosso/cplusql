#ident "file_id $Id: MatchCountExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MatchCountExpression.cpp
// Author:    mgrosso 
// Created:   Fri Nov  5 11:23:37 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: MatchCountExpression.cpp,v 1.1 2004/12/30 09:06:02 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "MatchCountExpression.h"

class MatchCountExpressionPriv 
{
    private:
    friend class MatchCountExpression ;
    ptr< list< char_ptr > >     strs_ ;
    ExpressionPtr               op_ ;
    long long                   count_;

    MatchCountExpressionPriv(
        ptr< list< char_ptr > > &matchlist, ExpressionPtr &operand 
    )
    :strs_(matchlist),op_(operand)
    {
        ;//noop
    };
};

MatchCountExpression::MatchCountExpression(
        ptr< list< char_ptr > > &matchlist, ExpressionPtr &operand 
)
:BinaryIntegerExpression(operand, operand )
{
    d_=new MatchCountExpressionPriv (matchlist, operand);
}

MatchCountExpression::~MatchCountExpression()
{
    delete d_;
}

void
MatchCountExpression::notify() 
{
    d_->op_->notify();
    const char *s = d_->op_->getRaw();
    d_->count_=0;
    while(*s)
    {
        list< char_ptr >::iterator i, e;
        for( i=d_->strs_->begin(), e=d_->strs_->end(); i!=e; ++i )
        {
            if( !strncmp( s, (*i).get(), strlen((*i).get())))
            {
                ++d_->count_;
            }
        }
        ++s;
    }
    IntegerExpression::notify();
}

long long
MatchCountExpression::getLongLong() const 
{
    return d_->count_;
}

