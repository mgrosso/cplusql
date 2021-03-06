#ident "file_id $Id: ascii_iso.cpp,v 1.2 2005/01/24 18:51:11 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// 
// File:      iso8859_1_to_ascii.cpp
// Author:    mgrosso 
// Created:   Fri Oct 29 10:30:59 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: ascii_iso.cpp,v 1.2 2005/01/24 18:51:11 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "ascii_iso.h"
#include "BaseException.h"



static const unsigned char identity [ 256 ]  = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 
    250, 251, 252, 253, 254, 255,
};

static const unsigned char iso8859_1_to_ascii_conversions[ 256 ]  = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 123, 124, 125, 126, 127, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 
    'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I', 'D', 'N', 
    'O', 'O', 'O', 'O', 'O', 32, 'O', 'U', 'U', 'U', 
    'U', 'Y', 'P', 's', 'a', 'a', 'a', 'a', 'a', 'a', 
    'a', 'c', 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i', 
    'd', 'n', 'o', 'o', 'o', 'o', 'o', 32, 'o', 'u', 
    'u', 'u', 'u', 'y', 'b', 'y' 
};

static const unsigned char alnum_white [ 256 ]  = {
    0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 32, 32, 
    32, 32, 32, 32, 32, 65, 66, 67, 68, 69, 
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
    90, 32, 32, 32, 32, 32, 32, 97, 98, 99, 
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32,
};

static const unsigned char alnum_null [ 256 ]  = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 0, 0, 
    0, 0, 0, 0, 0, 65, 66, 67, 68, 69, 
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
    90, 0, 0, 0, 0, 0, 0, 97, 98, 99, 
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0,
};

static const unsigned char ascii_tolower [ 256 ]  = {
    0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 32, 32, 
    32, 32, 32, 32, 32, 97, 98, 99, 100, 101, 
    102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 
    122, 32, 32, 32, 32, 32, 32, 97, 98, 99, 
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
    32, 32, 32, 32, 32, 32,
};

int
ascii_iso::convert_iso8859_1_to_ascii( int c )
{
    return convert( iso8859_1_to_ascii_conversions, c );
}

int
ascii_iso::convert_alnum_white_only( int c )
{
    return convert( alnum_white, c );
}

int
ascii_iso::convert_ascii_tolower( int c )
{
    return convert( ascii_tolower, c );
}

int 
ascii_iso::convert_non_alnum_null( int c )
{
    return convert( alnum_null, c );
}

int
ascii_iso::convert_identity( int c )
{
    return convert( identity, c );
}

const unsigned char *
ascii_iso::get_iso8859_1_to_ascii()
{
    return iso8859_1_to_ascii_conversions ;
}

const unsigned char *
ascii_iso::get_alnum_white_only()
{
    return alnum_white ;
}

const unsigned char *
ascii_iso::get_ascii_tolower()
{
    return ascii_tolower ;
}

const unsigned char *
ascii_iso::get_non_alnum_null()
{
    return alnum_null ;
}

const unsigned char *
ascii_iso::get_identity()
{
    return identity ;
}

int 
ascii_iso::convert( const unsigned char map[256], int c )
{
    if( c > 255  || c < 0 )
    {
        PANICV("iso8859-1 only has single byte characters, but c = %i", c );
    }
    return map[c];
}


