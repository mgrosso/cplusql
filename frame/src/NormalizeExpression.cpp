#ident "file_id $Id: NormalizeExpression.cpp,v 1.3 2005/01/24 18:38:16 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NormalizeExpression.cpp
// Author:    mgrosso 
// Created:   Fri Oct 29 03:42:37 EDT 2004 on tunerank
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: NormalizeExpression.cpp,v 1.3 2005/01/24 18:38:16 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "NormalizeExpression.h"
#include "ByteBuf.h"
#include "ascii_iso.h"

//setting this too large in order to temporarily work around a bug
#define NEBFSZ 32



class NormalizeExpressionPriv 
{
    private:
    friend class NormalizeExpression ;
    ExpressionPtr           op_ ;
    ptr< char_ptr_list >    stop_prefixes_;
    ptr< char_ptr_list >    stop_words_;

    ByteBuf     buf_ ;
    ByteBuf     buf2_ ;
    unsigned char map_ [256];
    char *    raw_;

    NormalizeExpressionPriv(
        ExpressionPtr &op, 
        ptr< char_ptr_list >    stop_prefixes,
        ptr< char_ptr_list >    stop_words
    )
    :op_(op),stop_prefixes_(stop_prefixes),stop_words_(stop_words),
        buf_(NEBFSZ,NEBFSZ),buf2_(NEBFSZ,NEBFSZ)
    {
        for( int i =0; i<256; ++i )
        {
            map_[i]=i;
        }
        stop_words_->push_back(ptr_strdup("  "));
    };
    char* trimends( char *s )
    {
        //skip leading spaces.
        while( *s  == 0x20 )
        {
            ++s;
        }
        //trim trailing spaces is a bit more work.  advance to closing
        //null, backup one, then change spaces to NULL as long as they are
        //spaces.
        char *end ; 
        for( end=s ; *end ; ++end ) {} 
        if( end != s )
        {
            --end; //move back one from trailing null.
        }
        for( ; end != s && *end == 0x20 ; --end )
        {
            *end = '\0';
        }
        return s;
    };
    void squeeze( const unsigned char *src, ByteBuf &dest, unsigned char w )
    {
        unsigned char prevchar = '\0';
        unsigned char c ;
        unsigned int position=0;
        for( c=*src; c ; c=*++src )
        {
            unsigned char cm = map_[ static_cast<int>(c) ] ;
            //fprintf(stderr,  "position %lu, c=%i:%c cm=%i:%c\n",
            //    position, c, c ? c : ' ', cm, cm ? cm : ' ' 
            //);
            ++position;
            if( c && cm && cm != w )
            {
                if( prevchar == w )
                {
                    dest.append( static_cast<char>(prevchar ));
                }
                dest.append( static_cast<char>(cm));
            }
            if( cm )
            {
                prevchar = cm ;
            }
        }
        dest.append( '\0' );
    };
    void remap( const unsigned char * changes )
    {
        //fprintf(stderr, "NormalizeExpression, char remappings:\n");
        for( int i =0; i<256; ++i )
        {
            //fprintf(stderr, "    %i(%c) was %i(%c) now %i(%c)\n", 
            //    i, i,
            //    map_[i], map_[i],
            //    changes[map_[i]], changes[map_[i]] 
            //);
            map_[i] = changes [ map_[i] ];
        }
    };
    void notify()
    {
        op_->notify();
        buf_.reset();
        buf2_.reset();
        const unsigned char *s = reinterpret_cast<const unsigned char *>( op_->getRaw());
        //fprintf(stderr, "NormalizeExpression: before first mapped copy from %s buf1(%s)\n\nbuf2(%s)\n", s, buf_.get(), buf2_.get());
        squeeze( s, buf_, ' ' );
        raw_ = buf_.buf();
        //fprintf(stderr, "NormalizeExpression: after first mapped copy from %s to %s\n", s, raw_ );

        raw_ = trimends( raw_ );
        //fprintf(stderr, "NormalizeExpression: after first trimends: %s\n", raw_ );

        //get rid of stop prefixes
        list<char_ptr>::iterator i,e;
        for( i=stop_prefixes_->begin(), e=stop_prefixes_->end(); i!=e; ++i)
        {
            const char *prefxstop = i->get();
            size_t prefxlen = strlen( prefxstop );
            if(! strncmp( raw_, prefxstop, prefxlen ))
            {
                raw_ = raw_ + prefxlen;
                break;
            }
        }
        //fprintf(stderr, "NormalizeExpression: after prefixes: %s\n", raw_ );
        //now eliminate general stop words
        for( i=stop_words_->begin(), e=stop_words_->end(); i!=e; ++i)
        {
            const char *stopword = i->get();
            char *a = strstr( raw_, stopword );
            if( a )
            {
                size_t stoplen = strlen(stopword );
                //fprintf(stderr, "NormalizeExpression: buffer before *a=\\0: %s\n", raw_ );
                *a='\0';
                //fprintf(stderr, "NormalizeExpression: buffer after *a=\\0: %s\n", raw_ );
                buf2_.reset();
                buf2_.appendNoNull( raw_ );
                //fprintf(stderr, "NormalizeExpression: buffer 2 is :%s\n", buf2_.get() );
                buf2_.append(' ');
                buf2_.append(a+stoplen);
                //fprintf(stderr, "NormalizeExpression: buffer 2 is : %s\n", buf2_.get());
                buf_.reset( buf2_.get());
                raw_ = buf_.buf();
                //fprintf(stderr, "NormalizeExpression: reset buffer to: %s\n", raw_ );
            }
        }
        raw_ = trimends( raw_ );
        buf2_.reset();
        squeeze( reinterpret_cast<const unsigned char *>( raw_ ), buf2_, ' ' );
        raw_ = buf2_.buf();
        //fprintf(stderr, "NormalizeExpression: after final trimends: %s\n", raw_ );
    };
};

NormalizeExpression::NormalizeExpression( 
    ExpressionPtr &operand,
    ptr< list < char_ptr > >   prefix_stop_words,
    ptr< list < char_ptr > >   stop_words,
    bool    skip_punctuation
)
{
    d_=new NormalizeExpressionPriv(operand,prefix_stop_words,stop_words);
    d_->remap( ascii_iso::get_iso8859_1_to_ascii() );
    if( skip_punctuation )
    {
        d_->remap( ascii_iso::get_non_alnum_null() );
    }else
    {
        d_->remap( ascii_iso::get_alnum_white_only() );
    }
    d_->remap( ascii_iso::get_ascii_tolower() );
}

NormalizeExpression::~NormalizeExpression()
{
    delete d_;
}

const char *
NormalizeExpression::getRaw()
{
    return d_->raw_;
}

const char *
NormalizeExpression::getRaw() const 
{
    return d_->raw_;
}

void
NormalizeExpression::notify()
{
    d_->notify();
}

