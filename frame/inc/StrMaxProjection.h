
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrMaxProjection.h
// Author:    mgrosso 
// Created:   Mon Jul 17 14:39:22 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRMAXPROJECTION_H
#define STRMAXPROJECTION_H 1

#include "StrMinProjection.h"

class StrMaxProjection : public StrMinProjection
{
    public:
    StrMaxProjection(ExpressionPtr &operand);
    virtual ~StrMaxProjection();
    virtual     AbstractProjectionPtr        copy();

    protected:
    int compare(const char *lhs, const char *rhs);

    private:
    //not implemented
    StrMaxProjection();
    StrMaxProjection( const StrMaxProjection &rhs );
    StrMaxProjection &operator=( const StrMaxProjection &rhs );
};

#endif /* STRMAXPROJECTION_H */
