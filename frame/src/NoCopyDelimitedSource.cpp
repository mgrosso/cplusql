#ident "file_id $Id: NoCopyDelimitedSource.cpp,v 1.14 2005/03/11 19:50:17 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NoCopyDelimitedSource.cpp
// Author:    mgrosso 
// Created:   Sun Apr 27 18:15:05 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: NoCopyDelimitedSource.cpp,v 1.14 2005/03/11 19:50:17 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "NoCopyDelimitedSource.h"
#include "ByteSource.h"
#include "Convert.h"
#include "BaseException.h"
#include "AppLog.h"
#include "Convert.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

//change the 0 to 1 if you want to print out a debug statement for every
//character parsed.
#if 0
#define NOCOPY_DEBUG
#endif

class NoCopyDelimitedSourcePriv 
{
    private:
    friend class NoCopyDelimitedSource ;


    enum charAction { enull=0, enoop, edelim, eline };
    charAction          actions_[ UCHAR_MAX + 1 ];

    NoCopyDelimitedSourcePriv(
       ByteSourcePtr &bsp, size_t buflen, size_t fields, char delim, size_t badparse_maxrows 
    )
    :bsp_(bsp),fields_(fields),delim_(delim),badparse_maxrows_(badparse_maxrows),
        badparse_rows_(0),last_badrow_(0),fields_found_(0),check_fields_(true), linenum_(0), bytes_(0), done_(false), badrow_(false)
    {
        setSize( buflen );  //initializes buf_, buflen_, end_

        bufp_=buf_;
        line_start_=buf_;
        column_start_=buf_;
        cursor_=buf_;

        if( fields_ == 0 )
        {
            check_fields_=false;
            fields_= 1;
        }
        pfields_=new char *[fields_];

        for( unsigned char i=0; i < UCHAR_MAX ; ++i )
        {
            actions_[ i ] = enoop;
        }
        // setting 255 value outside loop is necessary because i wraps around.
        actions_[ 255 ]=enoop ;
        // CAUTION:
        // the order in which we populate actions_ matters below:
        // we set delim first, before we set enull and eline. That way if delim
        // is actually \0 or \n it will be like having no delimiter and only
        // one column, but still separated by newlines.
        actions_[ static_cast< unsigned char>( delim )]=edelim ;
        actions_[ static_cast< unsigned char>('\0')]=enull ;
        actions_[ static_cast< unsigned char>('\n')]=eline ;
    };
    //ctor given variables
    ByteSourcePtr       bsp_;
    size_t              buflen_;
    size_t              fields_;    //number of fields to expect, or max number encountered 
                                    //during the lifetime of this object if we are not being 
                                    //strict.
    char                delim_;
    size_t              badparse_maxrows_ ;
    size_t              badparse_rows_ ;
    size_t              last_badrow_ ;
    //other internal state
    char_ptr            bufp_;      //delete []s buf_, ultimately 
    char *              buf_;       //where the action is.
    char *              end_;       //points to last byte of buf_.
    char *              line_start_;//beginning of current line_
    char *              column_start_;//beginning of current_column_
    char *              cursor_;    //points to last byte parsed.
    char **             pfields_;   //array of pointers to the beginnings of columns
    size_t              fields_found_; //number of fields found in this line.
    bool                check_fields_; //whether or not to insist on number of columns.
    size_t              linenum_;   //number of newlines parsed so far.
    size_t              bytes_;   //number of bytes parsed so far.
    bool                done_;      //true after bytesource returns false.
    bool                badrow_;

    //helpers
    inline void 
    setSize( size_t sz )
    {
        buflen_=sz;
        buf_=new char[sz] ;
        memset( buf_, '\0', sz );
        //we do the pointer arithmetic once here to make the realloc_if() 
        //function more efficient, since it will be called for every byte.
        end_=buf_+sz ;
    };
    inline void 
    realloc_if()
    {
        shift();            //try to reclaim space by shifting current line to start of buf.
        if( end_<=cursor_)  //still no space, must be big line or small buffer!
        {
            //double size of parse/read buffer.
            setSize( buflen_*2 );   //points buf_ to new memory of double size, bufp_has old.

            shift();                //works, because line_start, cursor_, and column_start
                                    //still point to old mem, which is still valid and "owned"
                                    //by bufp_.

            bufp_ = buf_;           //causes ptr to free old mem and "own" new mem.
        }
    };
    inline void
    shift(void)
    {
        //
        //its important to note that thare are different times when we get called.
        //in one case, we need to shift left in the same buffer. in the other case
        //the buffer has been reallocated and we are shifting to the new buffer. 
        //
        //in the second case buf_ points to the newly allocated buffer, so
        //we cant use it in our offset calculations. 
        //
        size_t column_offset = column_start_ - line_start_;
        size_t length = cursor_-line_start_;
        memmove( buf_, line_start_, length );
        for( size_t f=0; f< fields_found_; ++f )
        {
            pfields_[f]=buf_+ ( pfields_[f] - line_start_ );
        }
        line_start_=buf_;
        column_start_=line_start_ + column_offset;
        cursor_=line_start_+length;
        memset( cursor_, '\0', end_-cursor_ );
        *cursor_='\0';
    };
    inline void
    addField( char *f )
    {
        //fprintf( stderr, "NoCopyDelimitedSource::addField() col %s, address %x, line %s\n", f, (unsigned int)f, line_start_ );
        if( fields_found_>=fields_)
        {
            //
            //addField will be called very frequently, but its relatively rare that the if()
            //test above will suceed.
            //
            if( check_fields_ )
            {
                //we allready preallocated fields_ to the only acceptable length.
                if( !badrow_ && linenum_ != last_badrow_ )
                {
                    if( ++badparse_rows_ > badparse_maxrows_ )
                    {
                        throw BaseException::factory(
                            "%s:%i: next(), input line %u byte %u found %u columns, should be %u\n",
                            __FILE__, __LINE__, linenum_, bytes_, fields_found_+1, fields_
                        );
                    }
                    else
                    {
                        APPLOG_WARN( 
                            "input line %u byte %u found %u columns, should be only %u, first column is [%s], last column is [%s] encountered total of [%u] total badrows so far, max is %u\n",
                            linenum_, bytes_,
                            fields_found_, fields_,
                            line_start_, column_start_,
                            badparse_rows_, badparse_maxrows_ );
                    }
                    last_badrow_ = linenum_ ;
                    badrow_=true;
                }
                return;
            }
            //we support dynamic numbers of fields, so reallocate pfields
            char **npfields=new char *[fields_*2];
            size_t i;
            //copy existing column pointers
            for( i=0; i<fields_; ++i )
            {
                npfields[i]=pfields_[i];
            }
            //null fill the rest.
            for( ; i< fields_*2; ++i )
            {
                npfields[i]=NULL;
            }
            fields_=fields_*2;
            delete [] pfields_;
            pfields_=npfields;
        }
        pfields_[fields_found_++]=f;
    };
};

NoCopyDelimitedSource::NoCopyDelimitedSource( 
   ByteSourcePtr &bsp, size_t buflen, size_t fields, char delim, size_t badparse_maxrows
)
{
    d_=new NoCopyDelimitedSourcePriv ( bsp, buflen, fields, delim, badparse_maxrows);
}

NoCopyDelimitedSource::~NoCopyDelimitedSource()
{
    //fprintf(stderr, "~NoCopyDelimitedSource top %u\n", (unsigned )this);
    
    if(d_->pfields_)
    {
        delete [] d_->pfields_ ;
    }
    delete d_;
    //fprintf(stderr, "~NoCopyDelimitedSource top %u\n", (unsigned )this);
}

bool
NoCopyDelimitedSource::next()
{
    //preconditions:
    //  line_start_ points to first character of new line, or NULL
    //  cursor_     points to line_start_

    //postconditions
    //  we found either a new line and returned true or EOF and returned false. 
    //  line_start_ points to first character past \n, not yet written to buffer
    //  cursor_     points to line_start_
    
    if(d_->done_)
    {
        return false;
    }
    d_->fields_found_=0;
    d_->column_start_=d_->line_start_;
#ifdef NOCOPY_DEBUG
    size_t whilecnt=0;
#endif /* NOCOPY_DEBUG */
    while(1) //loop till we cant get any more data, or until we find a new line.
    {
        if( d_->end_<=d_->cursor_)  
        {
            d_->realloc_if();
        }
        //
        //the fprintf below is handy for debugging the parse if you change this code.
        //
#ifdef NOCOPY_DEBUG
        unsigned char c = static_cast<unsigned char >(*d_->cursor_) ;
        fprintf(stderr, "NoCopyDelimitedSource.cpp: line %u, bytes %u cursor %x %x %x linestart %x colstart %x fieldcnt %u whilecnt %u action %u %u\n",
                d_->linenum_, 
                d_->bytes_, 
                (size_t)d_->cursor_, 
                *d_->cursor_, 
                c,
                reinterpret_cast< size_t >( d_->line_start_), 
                reinterpret_cast< size_t >( d_->column_start_), 
                d_->fields_found_, 
                whilecnt++,
                d_->actions_[ static_cast<unsigned char >(*d_->cursor_) ] , 
                d_->actions_[c] 
        );
#endif /* NOCOPY_DEBUG */
        switch( d_->actions_[ static_cast<unsigned char >(*d_->cursor_) ] )
        {
            case NoCopyDelimitedSourcePriv::enull: //time to get more data.
                {
                    size_t bytesread;
                    bool n = d_->bsp_->next( d_->cursor_, bytesread, d_->end_ - d_->cursor_ );
                    if( ! n )
                    {
                        d_->done_ = true ;
                        return false;
                    }
                    //if( bytesread < d_->end_ - d_->cursor_ )
                    //{
                    //    d_->done_= true;  //dont read any more after this.
                    //}
                }
                break;
            case NoCopyDelimitedSourcePriv::edelim:
                if( d_->badrow_)
                {
                    * d_->cursor_++ ='\0';
                    ++d_->bytes_;
                    break;
                }
                * d_->cursor_++ ='\0';
                d_->addField( d_->column_start_ );
                d_->column_start_=d_->cursor_;
                ++d_->bytes_;
                break;
            case NoCopyDelimitedSourcePriv::eline:
                * d_->cursor_++ ='\0';
                if( d_->badrow_ )
                {
                    ++d_->linenum_;
                    d_->line_start_=d_->cursor_;
                    d_->column_start_=d_->cursor_;
                    ++d_->bytes_;
                    d_->badrow_ = false;
                    d_->fields_found_=0;
                    break;
                }

                d_->addField( d_->column_start_ );
                if( d_->check_fields_ && d_->fields_found_ < d_->fields_ )
                {
                    if( ++d_->badparse_rows_ > d_->badparse_maxrows_ )
                    {
                        throw BaseException::factory(
                            "%s:%i: next(), input line %u byte %u found only %u columns, should be %u, first column is [%s], last column is [%s] encountered total of [%u] total badrows\n",
                            __FILE__, __LINE__, d_->linenum_, d_->bytes_, 
                            d_->fields_found_, d_->fields_, 
                            d_->line_start_, d_->column_start_,
                            d_->badparse_rows_ );
                    }
                    else
                    {
                        APPLOG_WARN( 
                            "input line %u byte %u found only %u columns, should be %u, first column is [%s], last column is [%s] encountered total of [%u] total badrows so far, max is %u\n",
                            d_->linenum_, d_->bytes_,
                            d_->fields_found_, d_->fields_,
                            d_->line_start_, d_->column_start_,
                            d_->badparse_rows_, d_->badparse_maxrows_ );
                        d_->last_badrow_ = d_->linenum_ ;
                        ++d_->linenum_;
                        d_->line_start_=d_->cursor_;
                        d_->column_start_=d_->cursor_;
                        ++d_->bytes_;
                        break ;
                    }
                }
                ++d_->linenum_;
                d_->line_start_=d_->cursor_;
                d_->column_start_=d_->cursor_;
                ++d_->bytes_;
                return true;
                break;
            case NoCopyDelimitedSourcePriv::enoop: 
                //most common case, not a delimiter or new line or null. fall through.
                ++d_->cursor_ ;
                ++d_->bytes_;
                break;
            default:    //noop
                PANIC_I("internal error, you found a bug", d_->bytes_ );
                break;
        }
    }
    //not reached
    return false;
}


int                 
NoCopyDelimitedSource::lineNum()
{
    return d_->linenum_;
}

size_t              
NoCopyDelimitedSource::getNumColumns() const
{
    return d_->fields_;
}

const char *        
NoCopyDelimitedSource::getRaw( size_t index ) 
{
    //fprintf(stderr, "NoCopyDelimitedSource.cpp:getRaw() index %u, address %x\n", index, (unsigned int)d_->pfields_[index] );
    //clients can use getNumColumns() to do bounds checking if need be.
    //most clients dont need to pay that penalty.
    return d_->pfields_[index];

}

const char *        
NoCopyDelimitedSource::getRaw( size_t index ) const
{
    return d_->pfields_[index];
}

char_ptr            
NoCopyDelimitedSource::getCharPtr( size_t index ) const 
{
    return ptr_strdup( getRaw(index));
}

long int       
NoCopyDelimitedSource::getLong( size_t index ) const 
{
    return Convert::convertToLong( getRaw( index ));
}

long long int       
NoCopyDelimitedSource::getLongLong( size_t index ) const 
{
    return Convert::convertToLongLong( getRaw( index ));
}

double
NoCopyDelimitedSource::getDouble( size_t index ) const 
{
    return Convert::convertToDouble( getRaw( index ));
}

long double
NoCopyDelimitedSource::getLongDouble( size_t index ) const 
{
    return Convert::convertToLongDouble( getRaw( index ));
}

bool
NoCopyDelimitedSource::getBool( size_t index ) const 
{
    return Convert::convertToBool( getRaw( index ));
}

