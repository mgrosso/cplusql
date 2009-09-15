
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StrMinProjection.h
// Author:    mgrosso 
// Created:   Mon Jul 17 12:44:04 PDT 2006 on caliban
// Project:   
// Purpose:   
// 
// $Id$
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRMINPROJECTION_H
#define STRMINPROJECTION_H 1

#include "StringProjection.h"
    
class StrMinProjectionPriv ;
class StrMinProjection : public StringProjection
{
    public:
    StrMinProjection( ExpressionPtr & exp );
    virtual ~StrMinProjection();
    virtual     void                            childNotify();
    virtual     AbstractProjectionPtr        copy();

    protected:
    virtual     int compare(const char *lhs, const char *rhs);
    virtual     ExpressionPtr & getOperand();

    private:
    StrMinProjectionPriv *d_;
    //not implemented
    StrMinProjection();
    StrMinProjection( const StrMinProjection &rhs );
    StrMinProjection &operator=( const StrMinProjection &rhs );
};



#endif /* STRMINPROJECTION_H */
