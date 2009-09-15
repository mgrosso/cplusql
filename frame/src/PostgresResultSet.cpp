#ident "file_id $Id: PostgresResultSet.cpp,v 1.6 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      PostgresResultSet.cpp
// Author:    djain Deepak Jain,,,,mgrosso,00000000
// Created:   Fri Feb  8 13:40:49 PST 2002 on devbidb.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: PostgresResultSet.cpp,v 1.6 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifdef FRAME_USES_POSTGRES

#include "db/DBException.h"
#include "db/PostgresResultSet.h"
#include "db/PostgresStatement.h"

PostgresResultSet::PostgresResultSet(PostgresStatement* parent, PGresult* result, bool valid) 
: _parent(parent), _result(result), _valid(valid), _rowsProcessed(0) {

	_currentRow = new char_ptr_vec;
    if(!_result)
    {
        PANICV("Result invalid.");
    }
    int num_columns = PQnfields(_result);
    int i;
    for(i = 0; i < num_columns;i++)
    {
	colMeta meta;
	meta.colName = PQfname(_result, i);
	meta.columnLen = PQfsize(_result, i);
	meta.dataType = PQftype(_result, i);
	_meta.push_back(meta);
    }
    _num_rows = PQntuples(_result);
}

bool
PostgresResultSet::next()
{
    if(!_result)
    {
        PANICV("Result invalid.");
    }
    ORSMetaData::iterator beg = _meta.begin();
    ORSMetaData::iterator end = _meta.end();

    _currentRow->clear();
    if (beg == end) return false;
    if (_rowsProcessed == _num_rows) return false;

    // Fill our local Row data.
    for (unsigned int i=0; i< _meta.size(); ++i)
    {
        if (PQgetisnull(_result, _rowsProcessed, i))
	{
            _currentRow->push_back(ptr_strdup(""));
        }
	else
	{
            _currentRow->push_back(ptr_strdup(PQgetvalue(_result,  _rowsProcessed, i)));
        }
    }
    _rowsProcessed++;
    return true;
}

PostgresResultSet::~PostgresResultSet() {
	if(_result)
	{
		PQclear(_result);
	}
}

const char *
PostgresResultSet::colName(int idx) {
    if ((unsigned)idx < _meta.size()) {
        return (_meta[idx].colName).c_str();
    }
    PANICV("Column Index [%i] out of bounds[%i]! ", idx, (int)_meta.size());
}

int
PostgresResultSet::colLength(int idx) {
    if ((unsigned)idx < _meta.size()) {
        return _meta[idx].columnLen;
    }
    PANICV ("Column Index[%i] out of bounds[%i]! ", idx, _meta.size());
}

int
PostgresResultSet::colLength(const char * colname) {
    ORSMetaData::iterator beg = _meta.begin();
    ORSMetaData::iterator end = _meta.end();
    int idx = -1;
    while (beg != end) {
        ++idx;
        if ((*beg).colName.compare(colname) == 0) {
            return colLength(idx);
        }
        ++beg;
    }
    PANICV("No such column!  [%s]", colname);
}

int
PostgresResultSet::numColumns() {
    return _meta.size();
}

int
PostgresResultSet::rowsProcessed() {
    return _rowsProcessed;
}

bool
PostgresResultSet::isValid() {
    return _valid;
}

DBRowPtr
PostgresResultSet::getRow() {
    return _currentRow;
}

const char *
PostgresResultSet::getString(const char * colname) {
    ORSMetaData::iterator beg = _meta.begin();
    ORSMetaData::iterator end = _meta.end();
    int idx = -1;
    while (beg != end) {
        ++idx;
        if ((*beg).colName.compare(colname) == 0) {
            return getString(idx);
        }
        ++beg;
    }
    PANICV("No such column!  [%s]",colname);
}

const char *
PostgresResultSet::getString(int idx) {
    if (idx < 0 || (unsigned int) idx >= _meta.size() || (unsigned int)idx >= _currentRow->size()) {
        PANICV("Column Index[%i] out of bounds! [%i]", idx, (int)_meta.size());
    }
    return (*(_currentRow->begin()+idx)).get();
}

int PostgresResultSet::getErrorCode () const
{
  return _parent->getErrorCode ();
}

#endif /* FRAME_USES_POSTGRES */
