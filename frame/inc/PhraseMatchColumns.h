
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PhraseMatchColumns.h
// Author:    mgrosso 
// Created:   Tue Jan 11 20:01:13 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: PhraseMatchColumns.h,v 1.1 2005/01/24 19:40:23 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PHRASEMATCHCOLUMNS_H
#define PHRASEMATCHCOLUMNS_H 1

#include "ParsedColumns.h"
#include "Expression.h"
#include "ptr.h"
#include <list>
#include "ParsedColumns.h"
    
class PhraseMatchColumnsPriv ;
class PhraseMatchColumns : public ParsedColumns
{
    public:
    PhraseMatchColumns(
        ptr< list< char_ptr > > &matchlist, 
        ExpressionPtr &operand,
        const char *input_delim,
        const char *output_delim
    );
    virtual                     ~PhraseMatchColumns();
    virtual const char *        getField(unsigned field_index);
    virtual void                notify();

    //functions below adapted into ParsedColumns interface using
    //the efields enum.

    enum efields { eMATCH_COUNT=0, eMATCHES=1 };
    int                         getMatchCount();
    const char *                getMatches();
    void                        findMatches( const char *haystack );

    private:
    PhraseMatchColumnsPriv *d_;
    //not implemented
    PhraseMatchColumns();
    PhraseMatchColumns( const PhraseMatchColumns &rhs );
    PhraseMatchColumns &operator=( const PhraseMatchColumns &rhs );
};



#endif /* PHRASEMATCHCOLUMNS_H */
