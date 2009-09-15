#ident "file_id $Id: OracleResultSet.cpp,v 1.12 2005/02/07 01:31:26 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      OracleResultSet.cpp
// Author:    Lance Ball
// Created:   Mon Oct 11 17:24:20 EDT 1999 on casper.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: OracleResultSet.cpp,v 1.12 2005/02/07 01:31:26 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifdef HAVE_OCI_H

#include "oci.h"
#include "db/DBException.h"
#include "BaseException.h"
#include "db/OracleResultSet.h"
#include "db/OracleStatement.h"
#define COL_NAME_LEN 64

OracleResultSet::OracleResultSet(OracleStatement* parent, bool valid) 
: _parent(parent), _valid(valid), _rowsProcessed(-1) {

    ub4 counter = 1;
    sb4 parm_status;
    ub2 dtype;
    ub4 colnamelen;
    unsigned short columnlen;
    char colbuf[COL_NAME_LEN];
    sword status = 0;
    OCIParam *mypard;

    parm_status = OCIParamGet(_parent->getStatementHandle(), OCI_HTYPE_STMT, 
                              _parent->getErrorHandle(),
                              (dvoid **) &mypard, counter);

    while( _valid && parm_status == OCI_SUCCESS ) {
        colMeta meta;
        // Get the data type
        status = OCIAttrGet( (dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
                             (dvoid*) &dtype, (ub4*) 0, 
                             (ub4) OCI_ATTR_DATA_TYPE,
                             (OCIError*) _parent->getErrorHandle() );
        _parent->getOracleConnectionParent ()->checkErr((text*)_errmsg, _parent->getErrorHandle(), 
                                   status);
        meta.dataType = dtype;

        // Get the column name
        text *colname;
        OCIError *e= (OCIError*) _parent->getErrorHandle();
        status = OCIAttrGet( (dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
                             (dvoid**) &colname, (ub4*) &colnamelen, 
                             (ub4) OCI_ATTR_NAME,
                             e );
        if (! _parent->getOracleConnectionParent ()->checkErr( (text*)_errmsg, 
                                          _parent->getErrorHandle(), 
                                          status) ) {
            cerr << "ERR: Cannot retreive name for column" << counter << endl;
            meta.colName = "UNKNOWN";
        } else {
            strncpy(colbuf, (char*)colname, colnamelen);
            colbuf[colnamelen] = 0;
            meta.colName = colbuf;
        }
        // Get the column length
        status = OCIAttrGet( (dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
                             (dvoid*) &columnlen, (ub4*) 0, 
                             (ub4) OCI_ATTR_DATA_SIZE,
                             (OCIError*) _parent->getErrorHandle() );
        if (! _parent->getOracleConnectionParent ()->checkErr( (text*)_errmsg, 
                                          _parent->getErrorHandle(), 
                                          status) ) {
            cerr << "ERR: Cannot get length for column " << counter << endl;
            meta.columnLen = 0;
        } else {
            meta.columnLen = columnlen;
        }

        // Get the next descriptor
        ++counter;
        parm_status = OCIParamGet(_parent->getStatementHandle(), 
                                  OCI_HTYPE_STMT, 
                                  _parent->getErrorHandle(),
                                  (dvoid **) &mypard, counter);
        _meta.push_back(meta);
    } 

    ORSMetaData::iterator beg = _meta.begin();
    ORSMetaData::iterator end = _meta.end();
    counter = 1;
	_currentRow = new char*[_meta.size()];
    if (beg == end)
	{
		 PANICV( "No columns found.!");
	}
    while(beg != end && status != OCI_NO_DATA)
	{
        unsigned short len = (*beg).columnLen;
		_currentRow[counter-1] = new char[len+1];
        memset(_currentRow[counter-1], 0, len+1);

        // Define our data buffer
		sword status = define((ub4)counter,
						(dvoid*)_currentRow[counter-1],
						(sb4)len+1);
        if (! _parent->getOracleConnectionParent ()->checkErr( (text*)_errmsg, 
                                          _parent->getErrorHandle(), 
                                          status) )
		{
    		PANICV("ERR: (OCIDefineByPos): %s", _errmsg );
        }
        ++beg;
		++counter;
	}
}

void
OracleResultSet::reset()
{
	int i = 0;
    ORSMetaData::iterator beg = _meta.begin();
    ORSMetaData::iterator end = _meta.end();
    while(beg != end)
	{
		unsigned int len = (*beg).columnLen;
        memset(_currentRow[i], 0, len+1);
		++i;
		++beg;
	}
}

bool
OracleResultSet::next() {
	reset();
    sword status = fetch();
    bool gb= (status == OCI_NO_DATA) ? false : true ;
    _rowsProcessed++;
    return gb ;
}

sword
OracleResultSet::fetch()
{
    return OCIStmtFetch( _parent->getStatementHandle(), 
                           _parent->getErrorHandle(), 1, 
                           OCI_FETCH_NEXT, OCI_DEFAULT );
}

sword
OracleResultSet::define(ub4 counter, dvoid* val, sb4 len)
{
    	OCIDefine* defnp;
    	ub2 rlenp;
        return OCIDefineByPos( _parent->getStatementHandle(), &defnp, 
                                 _parent->getErrorHandle(),
                                 counter, 
                                 val,
                                 len, SQLT_STR, 0, &rlenp, 0, 
                                 OCI_DEFAULT );
}
	
OracleResultSet::~OracleResultSet() {
	for(unsigned int counter = 0; counter < _meta.size(); ++counter)
	{
        delete _currentRow[counter];
	}
	delete _currentRow;
}

const char *
OracleResultSet::colName(int idx) {
    if ((unsigned)idx < _meta.size()) {
        return (_meta[idx].colName).c_str();
    }
    PANICV("Column Index out of bounds! 0");
    return "";
}

int
OracleResultSet::colLength(int idx) {
    if ((unsigned)idx < _meta.size()) {
        return _meta[idx].columnLen;
    }
    return -1;
}

int
OracleResultSet::colLength(const char * colname) {
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
    PANICV("No such column!  [%s]",  colname );
}

int
OracleResultSet::numColumns() {
    return _meta.size();
}

int
OracleResultSet::rowsProcessed() {
    return _rowsProcessed;
}

bool
OracleResultSet::isValid() {
    return _valid;
}

DBRowPtr
OracleResultSet::getRow() {
    DBRowPtr row = new char_ptr_vec;
	for(unsigned int counter = 0; counter < _meta.size(); ++counter)
	{
        row->push_back(ptr_strdup(_currentRow[counter]));
	}
	return row;
}

const char *
OracleResultSet::getString(const char * colname) {
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
    PANICV("No such column!  [%s]", colname );
}

const char *
OracleResultSet::getString(int idx) {
    if (idx < 0 || (unsigned int) idx >= _meta.size()) {
        PANICV("Column Index [%i] out of bounds [%i]! ", idx, (int)_meta.size());
    }
    return _currentRow[idx];
}

int OracleResultSet::getErrorCode () const
{
  return _parent->getErrorCode ();
}

#endif /* HAVE_OCI_H */
