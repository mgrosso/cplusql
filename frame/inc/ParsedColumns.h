
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ParsedColumns.h
// Author:    mgrosso 
// Created:   Thu May 13 23:08:32 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: ParsedColumns.h,v 1.1 2004/05/18 04:33:42 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef PARSEDCOLUMNS_H
#define PARSEDCOLUMNS_H 1

#include "Listener.h"
#include "Expression.h"
#include "ptr.h"
#include "frame_config.h"

class ParsedColumns : public Listener
{
    public:
    virtual                     ~ParsedColumns();
    virtual const char *        getField(unsigned field_index)=0;
    virtual void                notify()=0;

    private:
};

typedef ptr< ParsedColumns > ParsedColumnsPtr ;

#endif /* PARSEDCOLUMNS_H */
