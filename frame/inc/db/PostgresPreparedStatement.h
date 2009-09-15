// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresPreparedStatement.cpp
// Author:    djain Deepak Jain,,,,mgrosso,00000000
// Created:   Fri Feb 22 08:45:18 PST 2002 on devbidb.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresPreparedStatement.h,v 1.4 2004/09/21 18:05:01 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef POSTGRESPREPAREDSTATEMENT_H
#define POSTGRESPREPAREDSTATEMENT_H 1

#include <string>
#include "db/DBPreparedStatement.h"
#include "db/PostgresStatement.h"

class PostgresPreparedStatement :
				public DBPreparedStatement,
				public PostgresStatement
{
	public:
		virtual ResultSetPtr executeQuery();
		virtual int executeUpdate();
		virtual void setString(int paramIndex, const char *);
		virtual void setLong(int paramIndex, long); 
		virtual void setNumeric(int paramIndex, long long); 
		virtual void setInt(int paramIndex, int);
		virtual ResultSetPtr executeQuery(const char *);
		virtual int executeUpdate(const char*);
		virtual int getErrorCode() const;
		virtual ~PostgresPreparedStatement();

		PostgresPreparedStatement(PostgresConnection *, const char *);
	protected:

	private:
		PostgresPreparedStatement();	//Not implemented
		ptr<char> buildQuery();
		void set(int paramIndex, const char *);
		void clearFillers();
		int _num_fillers;
		string * _template_sql;
		string * _filler_strings;
		string _sql;

	friend class PostgresConnection;
};

#endif
