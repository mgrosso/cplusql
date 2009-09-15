
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NormalizeExpression.h
// Author:    mgrosso 
// Created:   Fri Oct 29 03:42:37 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: NormalizeExpression.h,v 1.2 2005/01/24 18:38:15 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef NORMALIZEEXPRESSION_H
#define NORMALIZEEXPRESSION_H 1

#include "ptr_strdup.h"
#include "RawExpression.h"

class NormalizeExpressionPriv ;
class NormalizeExpression : public RawExpression
{
    public:
    NormalizeExpression( 
        ExpressionPtr &operand,
        ptr< list < char_ptr > >   prefix_stop_words,
        ptr< list < char_ptr > >   stop_words ,
        bool skip_punctuation = false
    );
    virtual ~NormalizeExpression();
    virtual void notify();
    virtual const char * getRaw();
    virtual const char * getRaw() const ;

    private:
    NormalizeExpressionPriv *d_;
    //not implemented
    NormalizeExpression();
    NormalizeExpression( const NormalizeExpression &rhs );
    NormalizeExpression &operator=( const NormalizeExpression &rhs );
};

#endif /* NORMALIZEEXPRESSION_H */
