// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      FileExistsExpression.h
// Author:    holahrei Howard Olah-Reiken
// Created:   Sat Nov  9 21:55:53 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: FileExistsExpression.h,v 1.2 2004/03/23 01:43:00 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef FILEEXISTSEXPRESSION_H
#define FILEEXISTSEXPRESSION_H 1

#include "BooleanExpression.h"
#include "UnaryExpression.h"

class FileExistsExpression : public BooleanExpression
{
    public:
    FileExistsExpression( ExpressionPtr &filename );
    FileExistsExpression( const char* filename );
    virtual ~FileExistsExpression();
    virtual void                notify() ;
    virtual bool                isNull() const ;
    virtual bool                getBool() const ;

    protected:
    private:
    ExpressionPtr   filename_ ;
    bool            fileExists_;
    //not implemented
    FileExistsExpression();
    FileExistsExpression(const FileExistsExpression &rhs);
    FileExistsExpression &operator=(const FileExistsExpression &rhs);
};

#endif /* FILEEXISTSEXPRESSION_H */
