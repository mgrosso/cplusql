
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SQLValueExpression.h
// Author:    mgrosso 
// Created:   Sun Jan  9 22:01:15 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: SQLValueExpression.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SQLVALUEEXPRESSION_H
#define SQLVALUEEXPRESSION_H 1

#include "db/DBConnection.h"
#include "RawExpression.h"

class SQLValueExpressionPriv ;
class SQLValueExpression : virtual public RawExpression
{
    public:
    SQLValueExpression(
        DBConnectionPtr &db,
        ExpressionPtr &sql,
        bool panicifnotfound
    );
    virtual                     ~SQLValueExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    SQLValueExpressionPriv *d_;
    //not implemented
    SQLValueExpression();
    SQLValueExpression( const SQLValueExpression &rhs );
    SQLValueExpression &operator=( const SQLValueExpression &rhs );
};

#endif /* SQLVALUEEXPRESSION_H */
