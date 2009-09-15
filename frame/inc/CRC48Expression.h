// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CRC48Expression.h
// Author:    djain Deepak Jain
// Created:   Thu Apr 14 09:00:47 EST 2003 on LS-11452
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CRC48Expression.h,v 1.2 2003/06/10 01:54:40 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CRC48EXPRESSION_H
#define CRC48EXPRESSION_H 1

#include "HashExpression.h"
#include "ptr.h"

class HashExpressionPriv  ;

class CRC48Expression : public HashExpression
{
    public:
    CRC48Expression( const ExpressionPtr &dsp );
    virtual ~CRC48Expression();

    protected:

    private:
    //not implemented
    CRC48Expression();
    CRC48Expression(const CRC48Expression &rhs);
    CRC48Expression &operator=(const CRC48Expression &rhs);
};

#endif /* CRC48EXPRESSION_H */
