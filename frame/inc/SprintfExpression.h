
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SprintfExpression.h
// Author:    mgrosso 
// Created:   Sat Jan  8 02:15:09 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SprintfExpression.h,v 1.1 2005/01/24 19:46:32 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SPRINTFEXPRESSION_H
#define SPRINTFEXPRESSION_H 1

#include "RawExpression.h"

class SprintfExpressionPriv ;
class SprintfExpression : public RawExpression
{
    public:
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;
    virtual ~SprintfExpression();
    SprintfExpression( const char *format, ExpressionList values );

    private:
    SprintfExpressionPriv *d_;
    //not implemented
    SprintfExpression();
    SprintfExpression( const SprintfExpression &rhs );
    SprintfExpression &operator=( const SprintfExpression &rhs );
};



#endif /* SPRINTFEXPRESSION_H */
