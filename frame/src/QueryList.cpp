#ident "file_id $Id: QueryList.cpp,v 1.1 2005/02/07 01:28:08 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      QueryList.cpp
// Author:    mgrosso 
// Created:   Sat Feb  5 19:12:40 EST 2005 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2005 
// 
// $Id: QueryList.cpp,v 1.1 2005/02/07 01:28:08 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "QueryList.h"
#include "db/DBStatement.h"
#include "db/DBResultSet.h"

ptr<list<char_ptr> > 
QueryList::makeList( DBConnectionPtr &db, const char *query )
{
    StatementPtr s= db->createStatement();
    ResultSetPtr rsp = s->executeQuery(query);
    ptr<list<char_ptr> >  gb = new list<char_ptr> ;
    while( rsp->next())
    {
        char_ptr v( ptr_strdup_wnull(rsp->getString( 0 )));
        gb->push_back(v);
    }
    return gb;
}

