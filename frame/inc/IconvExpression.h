
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IconvExpression.h
// Author:    mgrosso 
// Created:   Sun Dec 12 17:58:26 EST 2004 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: IconvExpression.h,v 1.1 2004/12/30 09:06:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ICONVEXPRESSION_H
#define ICONVEXPRESSION_H 1

#include "RawExpression.h"

class IconvExpressionPriv ;
class IconvExpression : public RawExpression
{
    public:
                                IconvExpression( ExpressionPtr &src, 
                                    const char *to, const char *from );
    virtual                     ~IconvExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    IconvExpressionPriv *d_;
    //not implemented
    IconvExpression();
    IconvExpression( const IconvExpression &rhs );
    IconvExpression &operator=( const IconvExpression &rhs );
};

#endif /* ICONVEXPRESSION_H */
