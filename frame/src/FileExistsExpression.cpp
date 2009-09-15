#ident "file_id $Id: FileExistsExpression.cpp,v 1.2 2004/03/23 01:43:01 holahrei Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NegationExpression.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 21:49:56 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: FileExistsExpression.cpp,v 1.2 2004/03/23 01:43:01 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "FileExistsExpression.h"
#include "LiteralExpression.h"
#include <sys/stat.h>

FileExistsExpression::FileExistsExpression( const char* filename )
:filename_(new LiteralExpression(filename))
{
    notify();
}

FileExistsExpression::FileExistsExpression( ExpressionPtr &filename )
:filename_(filename)
{
    ;//noop
}

FileExistsExpression::~FileExistsExpression()
{
    ;//noop
}

bool                
FileExistsExpression::isNull() const
{
    return filename_->isNull();
}

bool                
FileExistsExpression::getBool() const
{
    return fileExists_;
}

void                
FileExistsExpression::notify() 
{
    filename_->notify() ;
    struct stat buf;
    fileExists_ = (stat(filename_->getRaw(), &buf) == 0);
}

