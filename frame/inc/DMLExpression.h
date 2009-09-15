
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DMLExpression.h
// Author:    mgrosso 
// Created:   Fri Jan  7 14:47:58 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: DMLExpression.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DMLEXPRESSION_H
#define DMLEXPRESSION_H 1

#include "RawExpression.h"
#include "db/DBConnection.h"
    
class DMLExpressionPriv ;
class DMLExpression : public RawExpression
{
    public:
    DMLExpression(
            DBConnectionPtr &db,
            ExpressionList &beginsql,
            ExpressionList &sql, 
            ExpressionList &commitsql,
            size_t rows_per_transaction
    );
    virtual ~DMLExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;
    virtual void                notifyLast() ;

    private:
    DMLExpressionPriv *d_;
    //not implemented
    DMLExpression();
    DMLExpression( const DMLExpression &rhs );
    DMLExpression &operator=( const DMLExpression &rhs );
};



#endif /* DMLEXPRESSION_H */
