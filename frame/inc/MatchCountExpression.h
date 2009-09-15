
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MatchCountExpression.h
// Author:    mgrosso 
// Created:   Fri Nov  5 11:23:37 EST 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: MatchCountExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef MATCHCOUNTEXPRESSION_H
#define MATCHCOUNTEXPRESSION_H 1

#include "ptr_strdup.h"
#include "BinaryIntegerExpression.h"

class MatchCountExpressionPriv ;
class MatchCountExpression : public BinaryIntegerExpression
{
    public:
    MatchCountExpression( ptr< list< char_ptr > > &matchlist, ExpressionPtr &operand );
    virtual ~MatchCountExpression();
    virtual long long   getLongLong() const ;
    virtual void        notify() ;

    private:
    MatchCountExpressionPriv *d_;
    //not implemented
    MatchCountExpression();
    MatchCountExpression( const MatchCountExpression &rhs );
    MatchCountExpression &operator=( const MatchCountExpression &rhs );
};

#endif /* MATCHCOUNTEXPRESSION_H */
