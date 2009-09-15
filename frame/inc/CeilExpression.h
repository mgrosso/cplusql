// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CeilExpression.h
// Author:    djain Deepak Jain
// Created:   Thu Apr 14 09:00:47 EST 2003 on LS-11452
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CeilExpression.h,v 1.1 2004/05/05 23:03:14 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CEILEXPRESSION_H
#define CEILEXPRESSION_H 1

#include "HashExpression.h"
#include "ptr.h"

class HashExpressionPriv  ;

class CeilExpression : public HashExpression
{
    public:
    CeilExpression( const ExpressionPtr &dsp );
    virtual ~CeilExpression();

    protected:

    private:
    //not implemented
    CeilExpression();
    CeilExpression(const CeilExpression &rhs);
    CeilExpression &operator=(const CeilExpression &rhs);
};

#endif /* CEILEXPRESSION_H */
