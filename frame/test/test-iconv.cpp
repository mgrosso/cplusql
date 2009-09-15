#ident "file_id $Id: test-iconv.cpp,v 1.1 2004/12/31 21:57:16 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-iconv.cpp
// Author:    mgrosso 
// Created:   Sat Dec 11 23:55:41 EST 2004 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: test-iconv.cpp,v 1.1 2004/12/31 21:57:16 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <iconv.h>
#include "FileByteSource.h"
#include "BaseException.h"
#include "Expression.h"
#include "NormalizeExpression.h"
#include "CharExpression.h"

#define MYBUF 1024

int 
main( int argc, char **argv )
{
    try{
        char inbuf[MYBUF];
        char outbuf[MYBUF*2];
        size_t bytes_read;
        size_t bytes_2read;
        size_t bytes_2write;

        ExpressionPtr echar( new CharExpression( outbuf ));
        ExpressionPtr norm( new NormalizeExpression( echar,
            ptr< list< char_ptr> >( new list<char_ptr>()),
            ptr< list< char_ptr> >( new list<char_ptr>())
            ));

        FileByteSource in( 0, false );
        memset( inbuf, '\0', MYBUF );
        memset( outbuf, '\0', MYBUF*2 );

        iconv_t myconv  = iconv_open(  "ISO-8859-1", "UTF-8" );
        //iconv_t myconv  = iconv_open(  "UTF-8", "ISO-8859-1" );
        if( ! myconv )
        {
            PANICV("ouch, iconv_open failed.");
        }
        //inbuf[0]=0xf6;
        //inbuf[0]=0xc3;
        //inbuf[1]=0xb6;
        while( in.next( inbuf, bytes_read, MYBUF-1 ))
        {
            bytes_2read=bytes_read;
            bytes_2write = MYBUF * 2 ;
            char *ibufp=inbuf;
            char *obufp=outbuf;

            size_t r=iconv( myconv, &ibufp, &bytes_2read, &obufp, &bytes_2write );
            if( r==(size_t)(-1))
            {
                unsigned char i0=*ibufp;
                unsigned char i1=*(ibufp+1);
                unsigned char i2=*(ibufp+2);
                PANICV( "read=%lu inlen=%lu outlen=%lu ibufp[%s][%x,%x,%x] in[%s] out[%s]\n",
                    bytes_read, bytes_2read, bytes_2write, ibufp, i0, i1, i2, inbuf, outbuf );
            }
            fprintf( stderr, "iconv %lu, read=%lu inlen=%lu outlen=%lu in[%s] out[%s]\n",
                r, bytes_read, bytes_2read, bytes_2write, inbuf, outbuf );
            for( char *c = outbuf ; *c ; ++c )
            {
                size_t offset = c-outbuf;
                unsigned char cval = *c ;
                fprintf( stderr, "offset %lu, char=%x\n", offset, cval );
            }
            norm->notify();
            fprintf( stderr, "norm %s\n", norm->getRaw());

            memset( inbuf, '\0', MYBUF );
            memset( outbuf, '\0', MYBUF*2 );
        }
    }catch( std::exception &e )
    {
        fprintf( stderr, "exception %s", e.what() );
        return 1;
    }
    return 0;
}

