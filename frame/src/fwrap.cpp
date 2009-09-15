#ident "file_id $Id fwrap.cpp,v 1.3 2003/06/11 03:21:02 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      fwrap.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Jun  8 19:22:06 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: fwrap.cpp,v 1.20 2004/12/31 21:55:37 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define NEED_OPEN_FLAGS
#define NEED_GLOB_H
#include "frame_config.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fwrap.h"


#include "BaseException.h"



FILE *
fwrap::fopen( const char * filename, const char *mode )
{
    FILE *giveback ;
    if( ! filename || ! mode || !*filename || !*mode )
    {
        PANIC( "cant open NULL filename or NULL mode");
    }
    if( *filename == '-' )
    {
        if( *mode=='r' )
        {
            giveback=stdin;
        }else{
            giveback=stdout;
        }
    }
    else
    {
        giveback = ::fopen( filename, mode );
        if( ! giveback )
        {
            PANICV("Cant open file %s", filename );
        }
    }
    return giveback;
}

FILE *
fwrap::fdopen( int fd, const char *mode )
{
    FILE *giveback = NULL ;
    if( ! mode || !*mode || fd < 0 )
    {
        PANIC( "cant fdopen negative fd or NULL mode");
    }
    giveback = ::fdopen( fd, mode );
    if( ! giveback )
    {
        PANICV("fdopen returned null, cant fdopen file descripter=%i", fd );
    }
    return giveback;
}

flist_t
fwrap::fglob( const char * pattern, const char *mode )
{
    flist_t gb;
#ifndef HAVE_GLOB_H
    PANIC( "fwrap::fglob(): globs unsupported on this platform, for now.");
#else
    glob_t glb;
    memset( &glb, '\0', sizeof(glob_t));
    int flags = FRAME_GLOB_OPTS;
    int ret ;
    if( *pattern == '-' )
    {
        gb.push_back( fwrap::fopen( pattern, mode ));
    }
    else
    {
        if( 0!=( ret = glob( pattern, flags, NULL, &glb )))
        {
            PANICV( "fwrap::fglob(): glob failed %s", pattern );
        }
        for(unsigned int i=0; i< glb.gl_pathc ; ++i )
        {
            //gb.push_back( fwrap::fopen( glb.gl_pathv[ i ], mode ));
            gb.push_back(
                    fwrap::fdopen(fwrap::open_read(glb.gl_pathv[i]),mode));
        }
        globfree( &glb );
    }
    if( ! gb.size())
    {
        PANICV( "fwrap::fglob(): glob failed to find any files %s", pattern );
    }
#endif /* HAVE_GLOB_H */
    return gb;
}

flist_t
fwrap::fglobs( char_ptr_list &patterns, const char *mode )
{
    flist_t gb;
#ifndef HAVE_GLOB_H
    PANIC( "fwrap::fglob(): globs unsupported on this platform, for now.");
#else
    char_ptr_list::iterator i=patterns.begin();
    char_ptr_list::iterator e=patterns.end();
    for( ; i!=e; ++i )
    {
        flist_t gbt( fwrap::fglob( i->get(), mode));
        gb.splice( gb.end(), gbt );
    }
#endif /* HAVE_GLOB_H */
    return gb;
}

int 
fwrap::open_read(  const char * filename )
{
    if( !filename || !*filename )
    {
        PANIC("fwrap::open_read(), null filename arg()");
    }
    if( !strcmp( filename , "-" ))
    {
        return 0;
    }
    return fwrap::openfd( filename, OPEN_FOR_READING );
}

int 
fwrap::open_write(  const char * filename )
{
    if( !filename || !*filename )
    {
        PANIC("fwrap::open_write(), null filename arg()");
    }
    if( !strcmp( filename , "-" ))
    {
        return 1;
    }
    return fwrap::openfd( filename, OPEN_FOR_WRITING );
}

int 
fwrap::open_write(  const char * filename, bool o_excl, bool o_trunc )
{
    if( !filename || !*filename )
    {
        PANIC("fwrap::open_write(), null filename arg()");
    }
    if( !strcmp( filename , "-" ))
    {
        return 1;
    }
    //set both excl and trunc bits, and remove them 
    //if boolean args are false.
    int flags = OPEN_FOR_WRITING | O_EXCL | O_TRUNC ;
    if( ! o_excl  )
    {
        flags = flags ^ O_EXCL ;
    }
    if( !o_trunc )
    {
        flags = flags ^ O_TRUNC ;
        flags = flags | O_APPEND ;
    }
    return fwrap::openfd( filename, flags );
}

void 
fwrap::open_read_dup2(  const char * filename, int newfd )
{
    int f=fwrap::open_read( filename );
    if(-1==dup2( f, newfd ))                                          
    {                                                                   
        PANICV("cant dup2 input file %s", filename );
    }
}

void 
fwrap::open_write_dup2(  const char * filename, int newfd )
{
    int f=fwrap::open_write( filename );
    if(-1==dup2( f, newfd ))                                          
    {                                                                   
        PANICV("cant dup2 output file %s", filename );
    }
}

int 
fwrap::openfd(  const char * filename, int flags )
{
    if( !filename || !*filename )
    {
        PANIC("fwrap::openfd(), null filename arg()");
    }
    int fd = open( filename, flags, 0644 );
    if( fd==-1 )
    {
        PANICV("cant open file %s", filename );
    }
    return fd;
}


int
fwrap::openfd(const char *filename, int flags, int fileMode, int dirMode)
{
    int retVal = ::open(filename, flags, fileMode);

    if (retVal == -1)
    {
        if (isDirMissing(errno))
        {
            int st = fwrap::mkdirp(filename, dirMode);

            if (st == -1)
            {
                PANICV("can't mkdirp %s", filename);
            }

            retVal = ::open(filename, flags, fileMode);
        }
    }

    return retVal;
}


int 
fwrap::mkstemp(  const char * temptempl, char_ptr &filename )
{
    if( !temptempl || !*temptempl )
    {
        PANIC("fwrap::mkstemp(), null filename arg()");
    }
    int mkstemp_retry=5;
    int buflen=strlen(temptempl)+1;
    char tempfilename[buflen];
    int fd = -1;

    strcpy(tempfilename, temptempl );
    fd = ::mkstemp( tempfilename );
    while( fd == -1 && --mkstemp_retry  && errno == EEXIST )
    {
        strcpy( tempfilename, temptempl );
        fd = ::mkstemp( tempfilename );
    }
    if(fd == -1 )
    {
        PANICV("fwrap::mkstemp(): couldnt create uniquely named temp file");
    }
    filename=ptr_strdup(tempfilename);
    return fd;
}


int
fwrap::mkdirp(char *s, int mode)
{
    int retVal;
    char *orig = s;
    char *sl = strrchr(orig, '/');

    if (sl == orig) {
        //Ignoring, since last / is at root
        retVal = 0;
    } else {
        if (sl != NULL)
        {
            *sl = '\0';
        }

        retVal = ::mkdir(orig, mode);
        if (retVal == -1)
        {
            if (errno == EEXIST)
            {
                // The entry exists.  We need to make sure it is a dirctory.
                //
                struct stat buf;
                int statStat = ::stat(orig, &buf);

                if ((statStat == 0) && S_ISDIR(buf.st_mode))
                {
                    retVal = 0;
                }
            }
            else if (fwrap::isDirMissing(errno))
            {
                retVal = mkdirp(orig, mode);
                if (retVal == 0)
                {
                    // We've made our parent; now make ourself.
                    retVal = ::mkdir(orig, mode);
                }
            }
        }
        if (sl != NULL)
        {
            *sl = '/';
        }
    }

    return retVal;
}


int
fwrap::mkdirp(const char *filename, int mode)
{
    char_ptr fn = ptr_strdup(filename);
    int retVal = mkdirp(fn.get(), mode);
    return retVal;
}


/**
This concats root, path, and suffix.  Either/all can be NULL/zero-length.
A '/' is inserted between root and path if needed.  If both root end with a /
and path begins with one, only one is kept.
root is not required to begin with a /.
Caller is responsible for calling delete [].  NULL is never returned.

*/

char *
fwrap::mkFilePath(const char *root, const char *path, const char *sfx)
{
    size_t rootLen = 0;
    size_t pathLen = 0;
    size_t sfxLen = 0;

    if (root != (const char *) 0)
    {
        rootLen = strlen(root);
    }

    if (path != (const char *) 0)
    {
        pathLen = strlen(path);
    }

    if (sfx != (const char *) 0)
    {
        sfxLen = strlen(sfx);
    }

    size_t len = rootLen + 1 + pathLen + sfxLen + 1;
    char *retVal = new char[len];
    char *atNull = retVal;
    bool rootHasSlash = false;

    *atNull = '\0';

    if (rootLen > 0)
    {
        char lastC = root[rootLen - 1];

        (void) strcpy(atNull, root);
        atNull += rootLen;
        rootHasSlash = (lastC == '/');
    }
    else
    {
        rootHasSlash = true;
    }

    if (pathLen > 0)
    {
        bool pathHasSlash = (path[0] == '/');

        if (rootHasSlash && pathHasSlash)
        {
            path++;
            pathLen--;
        }
        else if (!rootHasSlash && !pathHasSlash)
        {
            (void) strcpy(atNull, "/");
            atNull += 1;
        }
        (void) strcpy(atNull, path);
        atNull += pathLen;
    }

    if (sfxLen > 0)
    {
        (void) strcpy(atNull, sfx);
    }

    return retVal;
}


int
fwrap::chmod(int fd, mode_t bitsToSet, mode_t bitsToClear)
{
    int retVal = 0;
    struct stat s;
    int st = ::fstat(fd, &s);

    if (st == 0)
    {
        mode_t newBits = (s.st_mode & (!bitsToClear)) | bitsToSet;

        st = ::fchmod(fd, newBits);
    }

    if (st == -1)
    {
        retVal = errno;
    }

    return retVal;
}


int
fwrap::chmod(const char *fname, mode_t bitsToSet, mode_t bitsToClear)
{
    int retVal = 0;
    struct stat s;
    int st = ::stat(fname, &s);

    if (st == 0)
    {
        mode_t newBits = (s.st_mode & (!bitsToClear)) | bitsToSet;

        st = ::chmod(fname, newBits);
    }

    if (st == -1)
    {
        retVal = errno;
    }

    return retVal;
}


ssize_t
fwrap::read(int fd, char *buf, size_t len, int eintrMax) throw ()
{
    ssize_t retVal = 0;

    if (buf == (char *) 0)
    {
        return -1 ;
    }
    int eintrCnt = 0;

    while (len > 0)
    {
        ssize_t numRd = ::read(fd, buf, len);

        if (numRd == 0) // EOF
        {
            break;
        }
        else if (numRd > 0)
        {
            retVal += numRd;
            buf += numRd;
            len -= numRd;
            eintrCnt = 0;
        }
        else
        {
            if ((errno == EINTR) && (eintrCnt < eintrMax))
            {
                ++eintrCnt;
            }
            else
            {
                if (retVal == 0)
                {
                    retVal = -1;
                }
                break;
            }
        }
    }
    return retVal;
}


ssize_t
fwrap::write(int fd, const char *buf, size_t len, int eintrMax) throw ()
{
    ssize_t retVal = 0;

    if (buf == (char *) 0)
    {
        return 0;
    }
    int eintrCnt = 0;
    while (len > 0)
    {
        ssize_t numWr = ::write(fd, buf, len);
        if (numWr >= 0)
        {
            retVal += numWr;
            buf += numWr;
            len -= numWr;
            eintrCnt = 0;
        }
        else
        {
            if ((errno == EINTR) && (eintrCnt < eintrMax))
            {
                ++eintrCnt;
            }
            else
            {
                if (retVal == 0)
                {
                    retVal = -1;
                }
                break;
            }
        }
    }
    return retVal;
}
