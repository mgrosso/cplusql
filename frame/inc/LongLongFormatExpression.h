
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      LongLongFormatExpression.h
// Author:    mgrosso 
// Created:   Mon Jan  3 19:30:36 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: LongLongFormatExpression.h,v 1.1 2005/01/04 06:45:34 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef LONGLONGFORMATEXPRESSION_H
#define LONGLONGFORMATEXPRESSION_H 1
#include "RawExpression.h"

#define DFLT_LLFORMAT "%lli"

class LongLongFormatExpressionPriv ;
class LongLongFormatExpression : public RawExpression
{
    public:
    LongLongFormatExpression( const char *format, ExpressionPtr &operand );
    LongLongFormatExpression( ExpressionPtr &operand );

    virtual ~LongLongFormatExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;
    virtual long double         getLongDouble() const;
    virtual long long           getLongLong() const;
    virtual double              getDouble() const;
    virtual long                getLong() const;

    private:
    LongLongFormatExpressionPriv *d_;
    //not implemented
    LongLongFormatExpression();
    LongLongFormatExpression( const LongLongFormatExpression &rhs );
    LongLongFormatExpression &operator=( const LongLongFormatExpression &rhs );
};

#endif /* LONGLONGFORMATEXPRESSION_H */
