
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MultiRegexColumns.h
// Author:    mgrosso 
// Created:   Tue Jun  8 22:22:05 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: MultiRegexColumns.h,v 1.3 2005/02/06 00:19:05 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef MULTIREGEXCOLUMNS_H
#define MULTIREGEXCOLUMNS_H 1

#include "ParsedColumns.h"
#include "ptr_strdup.h"

class MultiRegexColumnsPriv ;
class MultiRegexColumns : public ParsedColumns
{
    public:
    MultiRegexColumns( ExpressionPtr & source,
                      ptr< char_ptr_list > &patterns,
                      size_t fields,
                      bool case_insensitive,
                      bool posix_newlines
                      );

    virtual ~MultiRegexColumns();

    virtual const char *        getField(unsigned field_index);
    virtual void                notify();

    private:
    MultiRegexColumnsPriv *d_;
    //not implemented
    MultiRegexColumns();
    MultiRegexColumns( const MultiRegexColumns &rhs );
    MultiRegexColumns &operator=( const MultiRegexColumns &rhs );
};

#endif /* MULTIREGEXCOLUMNS_H */
