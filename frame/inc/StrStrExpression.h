// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrStrExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 23:19:04 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2004
// 
// $Id: StrStrExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef STRSTREXPRESSION_H
#define STRSTREXPRESSION_H 1

#include "BinaryIntegerExpression.h"

class StrStrExpression : public BinaryIntegerExpression 
{
    public:
    StrStrExpression( ExpressionPtr haystack, ExpressionPtr needle );
    virtual ~StrStrExpression();
    virtual long long           getLongLong() const ;
    protected:
    private:
    //not implemented
    StrStrExpression();
    StrStrExpression(const StrStrExpression &rhs);
    StrStrExpression &operator=(const StrStrExpression &rhs);
};

#endif /* STRSTREXPRESSION_H */
