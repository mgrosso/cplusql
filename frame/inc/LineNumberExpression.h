
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LineNumberExpression.h
// Author:    mgrosso 
// Created:   Mon Apr 28 16:15:00 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: LineNumberExpression.h,v 1.3 2003/06/17 20:54:14 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LINENUMBEREXPRESSION_H
#define LINENUMBEREXPRESSION_H 1

#include "IntegerExpression.h"
#include "AbstractRelation.h"

class LineNumberExpressionPriv ;
class LineNumberExpression : public IntegerExpression
{
    public:
    LineNumberExpression( RelationPtr &src );
    virtual                     ~LineNumberExpression();
    virtual long long           getLongLong() const ;

    private:
    LineNumberExpressionPriv *d_;
    //not implemented
    LineNumberExpression();
    LineNumberExpression( const LineNumberExpression &rhs );
    LineNumberExpression &operator=( const LineNumberExpression &rhs );
};

#endif /* LINENUMBEREXPRESSION_H */
