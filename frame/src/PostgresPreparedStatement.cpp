#ident "file_id $Id: PostgresPreparedStatement.cpp,v 1.9 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresPreparedStatement.cpp
// Author:    djain Deepak Jain,,,,mgrosso,00000000
// Created:   Fri Feb 22 08:45:05 PST 2002 on devbidb.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresPreparedStatement.cpp,v 1.9 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifdef FRAME_USES_POSTGRES

#include <vector>
#include <string.h>
#include "db/DBException.h"
#include "db/PostgresPreparedStatement.h"

#define MAX_QUERY_LEN 1024
#define MAX_ULONG_LEN 28

PostgresPreparedStatement::PostgresPreparedStatement(PostgresConnection * conn, const char * sql)
		:PostgresStatement(conn),
		_sql(sql)
{
	vector<string> template_sql;
	string::const_iterator iter = _sql.begin();
	bool in_quotes = false;
	int lastParmEnd = 0, i = 0;
	while(iter != _sql.end())
	{
		char c = *iter;
		if( c=='\'')
		{
			in_quotes = !in_quotes;
		}
		else if( c == '?' && !in_quotes)
		{
			template_sql.push_back(_sql.substr(lastParmEnd, (i-lastParmEnd)));
			lastParmEnd = i + 1;
		}
		++i;
		++iter;
	}
	template_sql.push_back(_sql.substr(lastParmEnd, (_sql.length() - lastParmEnd)));
	_num_fillers = template_sql.size() - 1;
	_template_sql = new string[_num_fillers + 1];
	_filler_strings = new string[_num_fillers];

	for(i = 0; i < _num_fillers; ++i)
	{
		_filler_strings[i] = string("");
		_template_sql[i] = template_sql[i];
	}
	_template_sql[_num_fillers] = template_sql[_num_fillers];
#ifdef DEBUG
	cerr << "Raw sql is [" << _sql.c_str() << "]" << endl;
#endif
}

ResultSetPtr
PostgresPreparedStatement::executeQuery()
{
	ptr<char> query = buildQuery();
#ifdef DEBUG
	cerr << "Prepared query is: " << query.get() << endl;
#endif
	ResultSetPtr set = executeQuery(query.get());
	clearFillers();
	return set;
}

ptr<char>
PostgresPreparedStatement::buildQuery()
{
	ptr<char> query = new char[MAX_QUERY_LEN];
	memset(query.get(), 0, MAX_QUERY_LEN);
	int i;
	int total_len = 0;
	for(i = 0; i < _num_fillers; ++i)
	{
		int len = _template_sql[i].length();
		total_len += len;
		if(total_len > MAX_QUERY_LEN)
		{
			PANICV("Prepared statement exceeds the max query length.");
		}
		strncat(query.get(), _template_sql[i].c_str(), len);
		if(_filler_strings[i].size() == 0)
		{
			PANICV("All the parameters have not been initialized.");
		}

		len = _filler_strings[i].length();
		total_len += len;
		if(total_len > MAX_QUERY_LEN)
		{
			PANICV("Prepared statement exceeds the max query length.");
		}
		strncat(query.get(), _filler_strings[i].c_str(), len);
	}
	int len = _template_sql[_num_fillers].length();
	total_len += len;
	if(total_len > MAX_QUERY_LEN)
	{
		PANICV("Prepared statement exceeds the max query length.");
	}
	strncat(query.get(), _template_sql[_num_fillers].c_str(), len);
	return query;
}

int
PostgresPreparedStatement::executeUpdate()
{
	ptr<char> query = buildQuery();
#ifdef DEBUG
	cerr << "Prepared query is: " << query.get() << endl;
#endif
	int retval = executeUpdate(query.get());
	clearFillers();
	return retval;
}

void
PostgresPreparedStatement::clearFillers()
{
	int i;
	for(i = 0; i < _num_fillers; ++i)
	{
		_filler_strings[i] = string("");
	}
}

void
PostgresPreparedStatement::set(int paramIndex, const char * val)
{
	if(paramIndex < 1 || paramIndex > _num_fillers)
	{
		PANICV("Index out of range.");
	}
	_filler_strings[paramIndex-1] = string(val);
}

void
PostgresPreparedStatement::setString(int paramIndex, const char * val)
{
	if(val == NULL)
	{
		set(paramIndex, "null");
	}
	else
	{
		string field = string("'") + val + string("'");
		set(paramIndex, field.c_str());
	}
}

void
PostgresPreparedStatement::setLong(int paramIndex, long val)
{
	/* keeping the format string the same as that
	 * of setNumeric so that we are not limited
	 * by the snprintf call, rather by the primitive
	 * type passed in as the arg(long or long long)
	 */
	char buf [MAX_ULONG_LEN];
	memset(buf, 0, MAX_ULONG_LEN);
	snprintf(buf, MAX_ULONG_LEN, "%li", val);
	set(paramIndex, buf);
}

void
PostgresPreparedStatement::setNumeric(int paramIndex, long long val)
{
	/* Need to explicitly cast a long long to numeric
	 * in postgres by the syntax:
	 *	col = CAST(val as numeric)
	 */
	char buf [MAX_ULONG_LEN];
	memset(buf, 0, MAX_ULONG_LEN);
	snprintf(buf, MAX_ULONG_LEN, "%lli", val);

	char ret [MAX_ULONG_LEN + 20];
	memset(ret, 0, MAX_ULONG_LEN);
	strcat(ret, "CAST(");
	strcat(ret, buf);
	strcat(ret, " as numeric)");
	set(paramIndex, ret);
}

void
PostgresPreparedStatement::setInt(int paramIndex, int val)
{
	char buf [MAX_ULONG_LEN];
	memset(buf, 0, MAX_ULONG_LEN);
	snprintf(buf, MAX_ULONG_LEN, "%i", val);
	set(paramIndex, buf);
}

ResultSetPtr
PostgresPreparedStatement::executeQuery(const char * sql)
{
	return PostgresStatement::executeQuery(sql);
}

int
PostgresPreparedStatement::executeUpdate(const char * sql)
{
	return PostgresStatement::executeUpdate(sql);
}

int
PostgresPreparedStatement::getErrorCode() const
{
	return PostgresStatement::getErrorCode();
}

PostgresPreparedStatement::~PostgresPreparedStatement()
{
	delete [] _template_sql;
	delete [] _filler_strings;
#ifdef DEBUG
	cerr << "PostgresPreparedStatement::~PostgresPreparedStatement() called." << endl;
#endif
}
#endif /* FRAME_USES_POSTGRES */
