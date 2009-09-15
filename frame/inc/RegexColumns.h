
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexColumns.h
// Author:    mgrosso 
// Created:   Thu May 13 22:46:51 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: RegexColumns.h,v 1.3 2005/02/06 00:19:05 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef REGEXCOLUMNS_H
#define REGEXCOLUMNS_H 1

#include "ParsedColumns.h"

class RegexColumnsPriv ;
class RegexColumns : public ParsedColumns
{
    public:
    virtual ~RegexColumns();
    RegexColumns ( ExpressionPtr & source,
                      const char *pattern,
                      size_t fields,
                      bool case_insensitive,
                      bool posix_newlines
                      );

    virtual const char *        getField(unsigned field_index);
    virtual void                notify();
 
    private:
    RegexColumnsPriv *d_;
    //not implemented
    RegexColumns();
    RegexColumns( const RegexColumns &rhs );
    RegexColumns &operator=( const RegexColumns &rhs );
};



#endif /* REGEXCOLUMNS_H */

