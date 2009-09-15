// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      JoinExpression.h
// Author:    djain Deepak Jain,,,,mgrosso,00000000
// Created:   Wed Apr 16 13:44:23 PDT 2003 on devbidb.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: JoinExpression.h,v 1.7 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef JOIN_EXPRESSION_H
#define JOIN_EXPRESSION_H 1

#include "RawExpression.h"

class JoinExpressionPriv;

class JoinExpression : public RawExpression
{
    public:
    JoinExpression( ExpressionList & list, const char * sepchars );
    virtual ~JoinExpression();
    virtual void                notify() ;
    virtual const char *        getRaw() ;
    virtual const char *        getRaw() const ;

    protected:

    private:
	JoinExpressionPriv* d_;
	char_ptr 					join();
    //not implemented
    JoinExpression();
    JoinExpression(const JoinExpression &rhs);
    JoinExpression &operator=(const JoinExpression &rhs);
};

#endif /* JOIN_EXPRESSION_H */
