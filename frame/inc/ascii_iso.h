
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      iso8859_1_to_ascii.h
// Author:    mgrosso 
// Created:   Fri Oct 29 10:30:59 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: ascii_iso.h,v 1.2 2005/01/24 18:51:11 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef ASCII_ISO_H
#define ASCII_ISO_H 1

class ascii_iso
{
    public:
    static int convert_iso8859_1_to_ascii( int c );
    static const unsigned char * get_iso8859_1_to_ascii();

    static int convert_alnum_white_only( int c );
    static const unsigned char * get_alnum_white_only();

    static int convert_ascii_tolower( int c );
    static const unsigned char * get_ascii_tolower();

    static int convert_non_alnum_null( int c );
    static const unsigned char * get_non_alnum_null();

    static int convert_identity( int c );
    static const unsigned char * get_identity();

    static int convert( const unsigned char map[256], int c );

    private:
    //not implemented
    ~ascii_iso();
    ascii_iso();
    ascii_iso( const ascii_iso &rhs );
    ascii_iso &operator=( const ascii_iso &rhs );
};



#endif /* ASCII_ISO_H */
