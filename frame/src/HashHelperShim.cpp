#ident "file_id $Id: HashHelperShim.cpp,v 1.1 2004/01/16 21:22:08 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      HashHelperShim.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sun Feb  9 19:46:12 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: HashHelperShim.cpp,v 1.1 2004/01/16 21:22:08 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "HashHelperShim.h"
#include "BaseException.h"

class HashHelperShimPriv 
{
    friend class HashHelperShim;
    private:
    HashHelperShimPriv( ExpressionPtr &e )
    :e_(e)
    {
        ;//noop
    };
    ExpressionPtr e_;
};

HashHelperShim::HashHelperShim(  ExpressionPtr e)
{
    d_=new HashHelperShimPriv( e);
    if(NULL==d_)
    {
        THROW_NOMEM ;
    }
}

char_ptr 
HashHelperShim::getHash() const
{
    return d_->e_->getCharPtr();
}

