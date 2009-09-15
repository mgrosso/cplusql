// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      fwrap.h
// Author:    mgrosso Matthew Grosso
// Created:   Sat Jun  8 19:20:02 EDT 2002 on circe
// Project:   
// Purpose:   simple classes to avoid typing the same thing every time
//          I open a file.
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: fwrap.h,v 1.11 2004/06/16 16:47:01 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef FWRAP_H
#define FWRAP_H 1

using namespace std ;

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <list>
#include "ptr_strdup.h"


typedef list< FILE * > flist_t ;

class fwrap 
{
    public:
    static FILE *   fopen( const char *filename, const char *mode );
    static FILE *   fdopen( int fd, const char *mode );
    static flist_t  fglob( const char *pattern, const char *mode );
    static flist_t  fglobs( char_ptr_list &patterns, const char *mode );
    static int      open_read( const char *filename );
    static int      open_write( const char *filename );
    static int      open_write( const char *filename, bool exclusive, bool truncate );
    static int      openfd( const char *filename, int flags );

	// This calls mkdirp(filename, dirNode) if necessary.
	static int      openfd( const char *filename, int flags, int mode, int dirMode);
    static void     open_read_dup2( const char *filename, int newfd );
    static void     open_write_dup2( const char *filename, int newfd );
    static int      mkstemp( const char *temptempl, char_ptr &filename );

	/** Same as sh's mkdir -p `dirname $filename` */
	static int      mkdirp( char *filename, int mode);

	/** This bounces over to the non-const version.  */
	static int      mkdirp( const char *filename, int mode);

	/**
	This concats root, path, and suffix.  Either/all can be NULL/zero-length.
	A '/' is inserted between root and path if needed. 
	Caller is responsible for calling delete [].  NULL is never returned.
	*/
	static char *   mkFilePath( const char *root, const char *path, const char *suffix);

	inline static bool isDirMissing(int errorNo) throw ();

	// Return 0 on success; errno otherwise.  Note: this does a stat first
	// to find out what the original bits are, so errno may be from a failed
	// stat.
	//
	static int      chmod(int fd, mode_t bitsToSet, mode_t bitsToClear);
	static int      chmod(const char *fname, mode_t bitsToSet, mode_t bitsToClear);

	// Return number of bytes read/written, or -1 if none and an error occured.
	// 
	static ssize_t  read(int fd, char *buf, size_t len, int eintrMax = 10) throw ();
	static ssize_t  write(int fd, const char *buf, size_t len, int eintrMax = 10) throw ();
};


bool
fwrap::isDirMissing(int errorNo) throw ()
{
	bool retVal = ((errorNo == ENOENT) || (errorNo == ENOTDIR));

	return retVal;
};


#endif /* FWRAP_H */
