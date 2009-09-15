// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NanosleepExpression.h
// Author:    mgrosso 
// Created:   Sat Jan  8 23:43:17 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: NanosleepExpression.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef NANOSLEEPEXPRESSION_H
#define NANOSLEEPEXPRESSION_H 1

#include "RawExpression.h"
#include "frame_config.h"

class NanosleepExpressionPriv ;
class NanosleepExpression : public RawExpression 
{
    public:
    NanosleepExpression( time_t seconds, long nanosecs );
    virtual ~NanosleepExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;
    private:
    NanosleepExpressionPriv *d_;
    //not implemented
    NanosleepExpression();
    NanosleepExpression( const NanosleepExpression &rhs );
    NanosleepExpression &operator=( const NanosleepExpression &rhs );
};

#endif /* NANOSLEEPEXPRESSION_H */
