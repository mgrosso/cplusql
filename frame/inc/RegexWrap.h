
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexWrap.h
// Author:    mgrosso 
// Created:   Thu May 13 10:25:34 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: RegexWrap.h,v 1.4 2005/01/24 18:49:11 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef REGEXWRAP_H
#define REGEXWRAP_H 1
#define NEED_REGEX
#include "frame_config.h"

class RegexWrap
{
    public:
    static void init( regex_t *r, const char *pattern, int flags );
    static bool match( regex_t *r, const char *operand ) ;
    static bool match( regex_t *r, const char *operand,
        size_t nmatch, regmatch_t pmatch [], int eflags ) ;
    static void release( regex_t *r ) ;
    private:
};



#endif /* REGEXWRAP_H */
