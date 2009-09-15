// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashHelperShim.h
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  9 19:39:03 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashHelperShim.h,v 1.1 2004/01/16 21:01:33 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef HASHHELPERSHIM_H
#define HASHHELPERSHIM_H 1

#include "Expression.h"
#include "HashHelper.h"

class HashHelperShimPriv ;

class HashHelperShim : public HashHelper
{
public:
    HashHelperShim(  ExpressionPtr e);
    virtual char_ptr getHash() const ;
private:
    HashHelperShimPriv  *d_;

    //not implemented
    HashHelperShim();
    HashHelperShim(const HashHelperShim &rhs);
    HashHelperShim &operator= (const HashHelperShim &rhs);
};

#endif /* HASHHELPERSHIM_H */
