#ident "file_id $Id: DBDelimitedSource.cpp,v 1.2 2005/01/24 19:09:43 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DBDelimitedSource.cpp
// Author:    mgrosso 
// Created:   Sun Apr 27 18:15:05 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: DBDelimitedSource.cpp,v 1.2 2005/01/24 19:09:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "db/DBDelimitedSource.h"
#include "Convert.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "db/DBConnection.h"
#include "db/DBStatement.h"
#include "db/DBResultSet.h"
#include "db/DBDelimitedSourcePriv.h"

DBDelimitedSourcePriv::DBDelimitedSourcePriv( 
    StatementPtr & stmt, ResultSetPtr &result
)
:stmt_(stmt),result_(result)
{
    //noop
}

DBDelimitedSourcePriv::DBDelimitedSourcePriv()
:stmt_(NULL),result_(NULL)
{
    //noop
}

DBDelimitedSource::DBDelimitedSource( 
   DBConnectionPtr & connection, const char * query
)
{
	StatementPtr stmt(connection->createStatement());
	ResultSetPtr rs(stmt->executeQuery(query));
    d_=new DBDelimitedSourcePriv ( stmt, rs );
}

DBDelimitedSource::DBDelimitedSource( 
   StatementPtr & stmt, ResultSetPtr & rs
)
{
    d_=new DBDelimitedSourcePriv ( stmt, rs );
}

DBDelimitedSource::DBDelimitedSource()
{
    d_=new DBDelimitedSourcePriv ();
}

DBDelimitedSource::~DBDelimitedSource()
{
    delete d_;
}

bool
DBDelimitedSource::next()
{
    if( !d_->result_.get())
    {
        return false;
    }
	return d_->result_->next();
}

int
DBDelimitedSource::lineNum()
{
    return d_->result_->rowsProcessed();
}

size_t
DBDelimitedSource::getNumColumns() const
{
    return d_->result_->numColumns();
}

const char *        
DBDelimitedSource::getRaw( size_t index ) 
{
    return d_->result_->getString( index );

}

const char *        
DBDelimitedSource::getRaw( size_t index ) const
{
    return d_->result_->getString( index );
}

char_ptr            
DBDelimitedSource::getCharPtr( size_t index ) const 
{
    return ptr_strdup( getRaw(index));
}

long int       
DBDelimitedSource::getLong( size_t index ) const 
{
    return Convert::convertToLong( getRaw( index ));
}

long long int       
DBDelimitedSource::getLongLong( size_t index ) const 
{
    return Convert::convertToLongLong( getRaw( index ));
}

double
DBDelimitedSource::getDouble( size_t index ) const 
{
    return Convert::convertToDouble( getRaw( index ));
}

long double
DBDelimitedSource::getLongDouble( size_t index ) const 
{
    return Convert::convertToLongDouble( getRaw( index ));
}

bool
DBDelimitedSource::getBool( size_t index ) const 
{
    return Convert::convertToBool( getRaw( index ));
}

