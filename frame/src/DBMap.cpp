#ident "file_id $Id: DBMap.cpp,v 1.6 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBMap.cpp
// Author:    djain 
// Created:   Sun Apr 27 18:15:05 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DBMap.cpp,v 1.6 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "db/DBMap.h"
#include "db/DBException.h"
#include "BaseException.h"
#include "RelationMeta.h"
#include "SingleRowDelimitedSource.h"
#include "db/DBDelimitedSource.h"
#include "db/DBPreparedStatement.h"
#include "db/DBConnection.h"

class DBMapPriv 
{
    private:
    friend class DBMap ;

	DBConnectionPtr conn_;
	RelationMetaPtr rel_;
	char_ptr query_template_;
};

DBMap::DBMap(
	RelationMetaPtr & rel,
	DBConnectionPtr & conn,
	const char * querytemplate
)
{
    d_=new DBMapPriv ();
	d_->conn_ = conn;
	d_->rel_ = rel;
	d_->query_template_ = ptr_strdup(querytemplate);
}

DBMap::~DBMap()
{
    delete d_;
}

DelimitedSourcePtr
DBMap::get_( ExpressionList & list )
{
    ExpressionListItr b;
    ExpressionListItr e;
	int i = 1;
	PreparedStatementPtr pstmt = d_->conn_->prepareStatement(d_->query_template_.get());
    for( b=list->begin(), e=list->end(); b!=e ; ++b )
    {
		pstmt->setString(i++,(*b)->getRaw());
	}

	ResultSetPtr rs(pstmt->executeQuery());
	StatementPtr stmt(pstmt);
	AbstractDelimitedSourceNextPtr a = new DBDelimitedSource( stmt, rs );
	if(!a->next())
	{
		PANICV("No rows found.");
	}
	DelimitedSourcePtr ds( a );
	return new SingleRowDelimitedSource( ds );
}

DelimitedSourcePtr
DBMap::get_( const char * key )
{
	PreparedStatementPtr pstmt = d_->conn_->prepareStatement(d_->query_template_.get());
	pstmt->setString(1,key);
	ResultSetPtr rs(pstmt->executeQuery());
	StatementPtr stmt(pstmt);
	AbstractDelimitedSourceNextPtr a = new DBDelimitedSource( stmt, rs );
	if(!a->next())
	{
		PANICV("No rows found.");
	}
	DelimitedSourcePtr ds( a );
	return new SingleRowDelimitedSource( ds );
}

bool
DBMap::exists_( const char * key )
{
	try
	{
		get_( key );
		return true;
	}catch(BaseException & e){
		return false;
	}
}

bool
DBMap::exists_( ExpressionList & list )
{
	try
	{
		get_( list );
		return true;
	}catch(BaseException & e){
		return false;
	}
}

size_t
DBMap::size_()
{
	return 1;
}

const char *
DBMap::getRelationName()const
{
	return d_->rel_->getRelationName();
}

char_ptr
DBMap::getRelationHeader()const
{
	return d_->rel_->getRelationHeader();
}

size_t
DBMap::getNumColumns()const
{
	return d_->rel_->getNumColumns();
}

const char *
DBMap::getColumnName( size_t index )const
{
	return d_->rel_->getColumnName(index);
}

size_t
DBMap::getColumnIndex( const char *col_name )const
{
	return d_->rel_->getColumnIndex(col_name);
}
