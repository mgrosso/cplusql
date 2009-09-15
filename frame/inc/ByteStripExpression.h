
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ByteStripExpression.h
// Author:    mgrosso 
// Created:   Mon Feb 21 19:07:25 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: ByteStripExpression.h,v 1.1 2005/02/22 07:19:16 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef BYTESTRIPEXPRESSION_H
#define BYTESTRIPEXPRESSION_H 1

#include "ptr_strdup.h"
#include "RawExpression.h"

class ByteStripExpressionPriv ;
class ByteStripExpression : public RawExpression
{
    public:
    ByteStripExpression( 
        ExpressionPtr &operand,
        const char *squeeze_chars,
        const char *skip_chars,
        const char *mapfrom,
        const char *mapto
    );
    virtual ~ByteStripExpression();
    virtual void notify();
    virtual const char * getRaw();
    virtual const char * getRaw() const ;

    private:
    ByteStripExpressionPriv *d_;
    //not implemented
    ByteStripExpression();
    ByteStripExpression( const ByteStripExpression &rhs );
    ByteStripExpression &operator=( const ByteStripExpression &rhs );
};



#endif /* BYTESTRIPEXPRESSION_H */
