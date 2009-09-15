// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DelimitedColumnExpression.h
// Author:    djain Deepak Jain
// Created:   Sat Feb 9 18:08:32 EST 2002 on sf-devdw002
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DelimitedColumn.h,v 1.3 2004/05/18 04:30:29 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef DELIMITEDCOLUMN_H
#define DELIMITEDCOLUMN_H 1

#include "ParsedColumns.h"

class DelimitedColumnPriv;

class DelimitedColumn : public ParsedColumns
{
    public:
    DelimitedColumn ( const ExpressionPtr & col,
                      const char *sepchars,
                      size_t fields,
                      bool expect_min,
                      bool no_extra = false );
    virtual                     ~DelimitedColumn();
    virtual const char *        getField(unsigned field_index);
    virtual void                notify();

    protected:

    private:
    void                        parse(const char*);
    DelimitedColumnPriv *d_;
    //not implemented
    DelimitedColumn();
    DelimitedColumn(const DelimitedColumn &rhs);
    DelimitedColumn &operator=(const DelimitedColumn &rhs);
};

#endif /* DELIMITEDCOLUMN_H */
