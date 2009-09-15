#ident "file_id $Id: CeilExpression.cpp,v 1.1 2004/05/05 23:02:02 bdonalds Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CeilExpression.cpp
// Author:    djain Deepak Jain
// Created:   Thu Apr 17 09:08:19 EST 2003 on LS-11452
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CeilExpression.cpp,v 1.1 2004/05/05 23:02:02 bdonalds Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BaseException.h"
#include "CeilExpression.h"
#include "HashExpressionPriv.h"
#include "HashAlgFactory.h"

CeilExpression::CeilExpression( const ExpressionPtr &dsp )
:HashExpression( dsp, 1 )
{
    d_->hash_=HashAlgFactory::getCeilHashPtr();
}

CeilExpression::~CeilExpression()
{
    ;//noop
}
