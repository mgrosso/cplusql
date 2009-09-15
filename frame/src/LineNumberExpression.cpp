#ident "file_id $Id: LineNumberExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LineNumberExpression.cpp
// Author:    mgrosso 
// Created:   Mon Apr 28 16:16:52 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: LineNumberExpression.cpp,v 1.5 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "LineNumberExpression.h"

    

class LineNumberExpressionPriv 
{
    private:
    friend class LineNumberExpression ;
    LineNumberExpressionPriv( RelationPtr &src )
    :src_(src)
    {
        //noop
    };
    RelationPtr src_;
};

LineNumberExpression::LineNumberExpression( RelationPtr &src )
{
    d_=new LineNumberExpressionPriv ( src );
}

LineNumberExpression::~LineNumberExpression()
{
    delete d_;
}

long long int
LineNumberExpression::getLongLong() const 
{
    return static_cast< long long int > ( d_->src_->lineNum());
}

