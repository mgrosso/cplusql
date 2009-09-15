// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashExpression.h
// Author:    mgrosso Matthew Grosso
// Created:   Thu Feb  6 07:00:47 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashExpressionPriv.h,v 1.2 2003/06/10 01:54:41 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef HASHEXPRESSIONPRIV_H
#define HASHEXPRESSIONPRIV_H 1

#include "Expression.h"
#include "AbstractHashAlg.h"
#include "ptr.h"

class HashExpressionPriv
{
  public:
    HashExpressionPriv( ptr< AbstractHashAlg > hash, const ExpressionPtr &e )
    :hash_(hash),e_(e)
    {
        ;//noop
    };
    ptr< AbstractHashAlg > hash_ ;
    const ExpressionPtr e_;
    char_ptr raw_;
};

#endif /* HASHEXPRESSIONPRIV_H */
