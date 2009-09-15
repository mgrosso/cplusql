// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Convert.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Nov  8 00:08:18 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: Convert.h,v 1.5 2005/04/25 17:43:54 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CONVERT_H
#define CONVERT_H 1

class Convert
{
    public:

    static bool                 isNull( const char *s );
    static bool                 convertToBool( const char *s );
    static int                  convertToInt( const char *s );
    static long int             convertToLong( const char *s );
    static double               convertToDouble( const char *s );
    static long long            convertToLongLong( const char *s );
    static long double          convertToLongDouble( const char *s );
    static unsigned long int    convertToUnsigned( const char *s );
    static unsigned long long int convertToUnsignedLongLong( const char *s );

    // s must be 2 bytes
    static void                 BoolToString( char *s, bool op );

    // s must be 32 bytes
    static void                 IntToString( char *s, int op );
    static void                 LongToString( char *s, long int op );
    static void                 DoubleToString( char *s, double op );
    static void                 LongLongToString( char *s, long long op );
    static void                 UnsignedToString( char *s, unsigned int op );
    // s must be 64 bytes
    static void                 LongDoubleToString( char *s, long double op );

};

#endif /* CONVERT_H */
