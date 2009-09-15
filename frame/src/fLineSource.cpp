#ident "file_id $Id: fLineSource.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      fLineSource.cpp
// Author:    mgrosso 
// Created:   Tue Apr 22 04:04:05 EDT 2003 on relay.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: fLineSource.cpp,v 1.4 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "fLineSource.h"
#include "BaseException.h"
#include "fwrap.h"
#include "Convert.h"
#include <stdio.h>
#include <string.h>

class fLineSourcePriv 
{
    private:
    friend class fLineSource ;
    FILE *      f_;             //byte source
    flist_t     fl_;
    char *      buf_;           //points to start of memory allocated.
    char *      end_;           //always points to last byte of memory allocated.
    size_t      buflen_;        //indicates amount of memory currently allocated.
    char *      line_start_;    //points to beginning of current line
    char *      column_start_;  //points to beginning of current line
    char *      cursor_;        //points to last byte read, 
                                //although it may have been overwritten with NULL.
    size_t      linenum_;       //count of lines read so far.

    bool        done_;          //if true, always return false after this.
    char **     pfields_;       //array of pointers to columns 
    size_t      fields_ ;       //theoretical count of columns and also size of pfields array
    size_t      fields_found_ ; //count of columns in pfields so far.
    bool        check_fields_ ; //whether or not to enforce theoretical column count.
    int         delim_;         //delimiter char. stored as int to reduce casting.

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
    popFile()
    {
        f_=fl_.front();
        fl_.pop_front();
    };

    inline void 
    realloc_if()
    {
        //ensure sufficient storage 
        if( end_ <= cursor_ )
        {
            //we have hit the end of the parse buffer, so shift left.
            shift();
            if(end_ <= cursor_ )
            {
                //we have encountered a line larger than the parse buffer, so double it.
                char *tbuf=buf_;
                setSize( buflen_*2 );
                shift();
                delete [] tbuf;
            }
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
    };

    inline bool
    addField( char *f )
    {
        if( fields_found_>=fields_)
        {
            //
            //addField will be called very frequently, but its relatively rare that the if()
            //test above will suceed.
            //
            if( check_fields_ )
            {
                //we allready preallocated fields_ to the only acceptable length.
                return false;
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
        return true;
    };

    fLineSourcePriv( flist_t &fl, size_t buflen, size_t fields, char delim )
    :fl_(fl),linenum_(0),done_(false),fields_(fields),fields_found_(0),delim_(delim)
    {
        //we call fglob, which might throw, before we call setSize(), so we dont leak.
        popFile();
        setSize( buflen );
        line_start_=buf_;
        column_start_=buf_;
        cursor_=buf_;
        if( fields_ > 0 )
        {
            check_fields_=true ;
        }else{
            fields_=1;
            check_fields_=false;
        }
        pfields_=new char *[fields_];
        for( size_t i=0; i<fields_; ++i )
        {
            pfields_[i]=NULL;
        }
        fields_found_=0;
    };
};

fLineSource::fLineSource( const char *filename_pattern, size_t buflen, size_t fields, char delim )
{
    flist_t fl=fwrap::fglob( filename_pattern, "r" ) ;
    d_=new fLineSourcePriv ( fl, buflen,fields, delim );
}

fLineSource::fLineSource( char_ptr_list &patterns, size_t buflen, size_t fields, char delim )
{
    flist_t fl=fwrap::fglobs( patterns, "r" ) ;
    d_=new fLineSourcePriv ( fl, buflen,fields, delim );
}

fLineSource::~fLineSource()
{
    if( d_->buf_ )
    {
        delete [] d_->buf_;
    }
    if( d_->pfields_ )
    {
        delete [] d_->pfields_ ;
    }
    delete d_;
}

bool
fLineSource::next()
{
    //preconditions:
    //  line_start_ points to first character of new line, not yet written to buffer
    //  cursor_     points to line_start_

    //postconditions
    //  we found either a new line and returned true or EOF and returned false. 
    //  line_start_ points to first character of new line, not yet written to buffer
    //  cursor_     points to line_start_
    
    if(d_->done_)
    {
        return false;
    }
    d_->fields_found_=0;
    bool nl=false;
    int i;
    while(1)
    {
        d_->realloc_if();
        i=fgetc(d_->f_);
        switch(i)
        {
            case EOF:
                //Note the last line must have a \n or it will be ignored.
                if( d_->fl_.size()==0)
                {
                    d_->done_=true;
                    return false;       
                }else
                {
                    d_->popFile();
                }
                break;
            case '\n':
                //We fall through on purpose to share column handling code for final column.
                //Other handling of new line is done below.
                nl=true;                    
            default: 
                if( i== d_->delim_ || nl )
                {
                    *d_->cursor_='\0';
                    if( !d_->addField( d_->column_start_ ))
                    {
                        //
                        //soon, we'll add the ability to handle poor data gracefully.
                        //that probably means we'll need to do some cleanup here so we dont
                        //break any invariants when we throw. likely that will mean advancing to
                        //the next new line, possibly after de-nulling and re-delimiting the 
                        //current line and writing it out to an error handler.
                        //
                        throw BaseException::factory(
                        "%s:%i: next(), too many columns, input line %i found %i, should be %i\n",
                        __FILE__, __LINE__, d_->linenum_, d_->fields_found_, d_->fields_ 
                        );
                    }
                    d_->column_start_=++d_->cursor_;
                }
                else
                {
                    *d_->cursor_++=static_cast<char>(i);
                }
                break;
        }

        //note:
        //at this point, cursor has been advanced by one to point at the next field,
        //and cursor_-1 has either a \0 or the fgetc contents.

        //if we hit a newline, then make sure that we have as many fields as we need.
        if(nl)
        {
            ++d_->linenum_;
            d_->line_start_=d_->cursor_;
            if( d_->check_fields_ && d_->fields_found_ < d_->fields_ )
            {
                throw BaseException::factory(
                    "%s:%i: next(), too few columns, input line %i found %i, should be %i\n",
                    __FILE__, __LINE__, d_->linenum_, d_->fields_found_, d_->fields_ );
            }
            return true;
        }
    }
    //not reached
    return false;
}


int                 
fLineSource::lineNum()
{
    return d_->linenum_;
}

char_ptr            
fLineSource::getCharPtr()
{
    charvec cv;
    size_t sz=d_->fields_ ;
    for( size_t i=0; i<sz; ++i )
    {
        cv.push_back( getRaw(i));
    }
    return ptr_strdup_list( cv, "|", false );
}

size_t              
fLineSource::getNumColumns() const
{
    return d_->fields_;
}

long long int       
fLineSource::getKey( size_t index ) const 
{
    return Convert::convertToLongLong( getRaw( index ));
}

const char *        
fLineSource::getRaw( size_t index ) 
{
//clients can use getNumColumns() to do bounds checking if need be.
//clients that use strict parsing probably dont need to pay that penalty.
//    if( index >= d_->fields_ )
//    {
//        PANIC_II("Index out of bounds", index, d_->fields_ );
//    }
    //fprintf( stderr, "fLineSource::getRaw() col %i : %s\n", index, d_->pfields_[index]);
    return d_->pfields_[index];

}

const char *        
fLineSource::getRaw( size_t index ) const
{
    //separate const version provided since parent class has non-const abstract function.
    //fprintf( stderr, "fLineSource::getRaw() col %i : %s\n", index, d_->pfields_[index]);
    return d_->pfields_[index];
}

char_ptr            
fLineSource::getCharPtr( size_t index ) const 
{
    return ptr_strdup( getRaw(index));
}

