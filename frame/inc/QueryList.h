
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      QueryList.h
// Author:    mgrosso 
// Created:   Sat Feb  5 19:12:40 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: QueryList.h,v 1.1 2005/02/07 01:28:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef QUERYLIST_H
#define QUERYLIST_H 1
#include "db/DBConnection.h"
    
class QueryList
{
    public:
    static ptr<list<char_ptr> > makeList(
        DBConnectionPtr &db, const char *query );

    private:
    //not implemented
    QueryList();
    QueryList( const QueryList &rhs );
    QueryList &operator=( const QueryList &rhs );
};

#endif /* QUERYLIST_H */
