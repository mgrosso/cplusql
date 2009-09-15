#ident "file_id $Id: CRC48Expression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CRC48Expression.cpp
// Author:    djain Deepak Jain
// Created:   Thu Apr 17 09:08:19 EST 2003 on LS-11452
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: CRC48Expression.cpp,v 1.5 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BaseException.h"
#include "CRC48Expression.h"
#include "HashExpressionPriv.h"
#include "HashAlgFactory.h"

CRC48Expression::CRC48Expression( const ExpressionPtr &dsp )
:HashExpression( dsp, 1 )
{
    d_->hash_=HashAlgFactory::getCRC48HashPtr();
}

CRC48Expression::~CRC48Expression()
{
    ;//noop
}
