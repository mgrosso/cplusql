
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DaemonizeExpression.h
// Author:    mgrosso 
// Created:   Sun Mar  6 20:55:41 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: DaemonizeExpression.h,v 1.1 2005/03/11 19:49:03 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DAEMONIZEEXPRESSION_H
#define DAEMONIZEEXPRESSION_H 1
#include "RawExpression.h"

class DaemonizeExpressionPriv ;
class DaemonizeExpression :public RawExpression
{
    public:
    DaemonizeExpression(bool dofork, const char *dir, const char *pidfile );
    virtual ~DaemonizeExpression() ;
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;
    virtual void                notify() ;

    private:
    DaemonizeExpressionPriv *d_;
    //not implemented
    DaemonizeExpression();
    DaemonizeExpression( const DaemonizeExpression &rhs );
    DaemonizeExpression &operator=( const DaemonizeExpression &rhs );
};



#endif /* DAEMONIZEEXPRESSION_H */
