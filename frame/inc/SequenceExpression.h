
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SequenceExpression.h
// Author:    mgrosso 
// Created:   Wed Nov  9 09:40:43 PST 2005 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SEQUENCEEXPRESSION_H
#define SEQUENCEEXPRESSION_H 1
#include "Expression.h"

class SequenceExpressionPriv ;
class SequenceExpression : public Expression
{
    public:
    SequenceExpression(
        ExpressionPtr   start,
        ExpressionPtr   increment,
        ExpressionPtr   reset);
    virtual ~SequenceExpression();

    virtual void notify() ;
    virtual long int            getLong() const ;
    virtual double              getDouble() const ;
    virtual long long           getLongLong() const ;
    virtual long double         getLongDouble() const ;

    virtual const char *        getRaw() ;

    virtual bool                isNull() const ;
    virtual char_ptr            getCharPtr() const ;
    virtual bool                getBool() const ;

    private:
    SequenceExpressionPriv *d_;
    //not implemented
    SequenceExpression();
    SequenceExpression( const SequenceExpression &rhs );
    SequenceExpression &operator=( const SequenceExpression &rhs );
};

#endif /* SEQUENCEEXPRESSION_H */
