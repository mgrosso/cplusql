
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PgListenExpression.h
// Author:    mgrosso 
// Created:   Sun Jan  9 04:21:31 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: PgListenExpression.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PGLISTENEXPRESSION_H
#define PGLISTENEXPRESSION_H 1

#include "RawExpression.h"
#include "db/DBConnection.h"

class PgListenExpressionPriv ;
class PgListenExpression : public RawExpression
{
    public:
    PgListenExpression( DBConnectionPtr &conn, const char *event );
    virtual                     ~PgListenExpression();
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    PgListenExpressionPriv *d_;
    //not implemented
    PgListenExpression();
    PgListenExpression( const PgListenExpression &rhs );
    PgListenExpression &operator=( const PgListenExpression &rhs );
};



#endif /* PGLISTENEXPRESSION_H */
