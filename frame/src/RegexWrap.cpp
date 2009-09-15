#ident "file_id $Id: RegexWrap.cpp,v 1.2 2004/05/18 04:46:47 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexWrap.cpp
// Author:    mgrosso 
// Created:   Thu May 13 10:28:19 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: RegexWrap.cpp,v 1.2 2004/05/18 04:46:47 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RegexWrap.h"
#include "BaseException.h"

#define REGEX_WRAP_ERRBUFSIZE 512

void 
RegexWrap::init( regex_t *r, const char *pattern, int flags )
{
    if( !pattern )
    {
        PANICV("null pattern arg"); 
    }
    int err = regcomp( r, pattern, flags );
    if( err != 0 )
    {
        char errbuf[REGEX_WRAP_ERRBUFSIZE ];
        (void)regerror( err, r, errbuf, REGEX_WRAP_ERRBUFSIZE );
        PANICV( "regcomp() could not compile [%s] flags %x regerror=%s\n", 
            pattern, flags, errbuf );
    }
}

bool 
RegexWrap::match( regex_t *r, const char *operand )
{
    return match( r, operand, 0, NULL, 0 );
}

bool
RegexWrap::match( regex_t *r, const char *operand,
        size_t nmatch, regmatch_t pmatch [], int eflags )
{
    if( 0 == regexec( r, operand, nmatch, pmatch, eflags ))
    {
        return true ;
    }
    return false ;
}

void 
RegexWrap::release( regex_t *r )
{
    regfree( r );
}

