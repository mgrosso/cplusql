#ident "file_id $Id: RegexColumns.cpp,v 1.4 2005/02/06 00:19:07 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RegexColumns.cpp
// Author:    mgrosso 
// Created:   Fri May 14 10:53:33 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: RegexColumns.cpp,v 1.4 2005/02/06 00:19:07 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RegexColumns.h"
#include "RegexWrap.h"
#include "Expression.h"
#include "ptr_strdup.h"
#include "BaseException.h"
#include "AppLog.h"

class RegexColumnsPriv 
{
    private:
    friend class RegexColumns ;
    regex_t             regex_ ;
    ExpressionPtr       operand_ ;
    char_ptr            pattern_ ;

    size_t              fields_ ;
    regmatch_t  *       matches_ ;

    const char **       colp_ ;
    char_ptr            data_ ;
    size_t              datalen_ ;

    static const char * empty ;

    RegexColumnsPriv(
        ExpressionPtr & source,
        const char *pattern,
        size_t fields,
        bool case_insensitive,
        bool posix_newlines
    );
    ~RegexColumnsPriv();
    void reset();
};

const char * RegexColumnsPriv::empty = "";

RegexColumnsPriv::RegexColumnsPriv(
    ExpressionPtr & source,
    const char *pattern,
    size_t fields,
    bool case_insensitive,
    bool posix_newlines
)
:operand_(source),pattern_(ptr_strdup_wnull(pattern)),
    fields_(fields),data_(NULL),datalen_(0)
{
    int flags = 
        REG_EXTENDED | 
        ( case_insensitive ? REG_ICASE : 0 ) |
#ifdef HAVE_PCREPOSIX_H
        ( posix_newlines ? PCRE_MULTILINE | PCRE_DOTALL : 0 ) 
#else
        ( posix_newlines ? 0 : REG_NEWLINE  )
#endif
        ;
    //flags |= PCRE_DOLLAR_ENDONLY ;
    //flags |= PCRE_DOTALL ;
    //flags |= PCRE_MULTILINE ;

    RegexWrap::init( &regex_, pattern_.get(), flags );
    colp_ = new const char *[fields +1 ];
    matches_ = new regmatch_t  [ fields ];
    reset();
    APPLOG_DBG("flags=0x%lx fields=%i pattern{%s}\n", flags, fields, pattern);
}

RegexColumnsPriv::~RegexColumnsPriv()
{
    RegexWrap::release( &regex_ );
    delete colp_;
    delete matches_;
}

void
RegexColumnsPriv::reset()
{
    colp_[0]="0";
    for( size_t i=1; i<fields_+1 ; ++i )
    {
        colp_[i]=empty;
    }
}

RegexColumns::RegexColumns(
    ExpressionPtr & source,
    const char *pattern,
    size_t fields,
    bool case_insensitive,
    bool posix_newlines
    )
{
    if( fields < 2 )
    {
        PANICV("The number of fields must be greater than or equal to 2, so that the first can be a boolean true if there is a match, and the second can match the whole part that matched the regex. The # fields may be bigger if you have subexpression that you want to use a new columns. fields=%i", 
        fields ); 
    }
    d_=new RegexColumnsPriv( source, pattern, fields, 
        case_insensitive, posix_newlines );
}

RegexColumns::~RegexColumns()
{
    delete d_;
}

void
RegexColumns::notify()
{
    d_->operand_->notify();
    bool matched = RegexWrap::match( &d_->regex_, d_->operand_->getRaw(),
        d_->fields_, d_->matches_, 0 );
    if( ! matched )
    {
        d_->reset();
        return ;
    }
    //yes, we matched.
    d_->colp_[0]="1";

    //
    //first ensure that data_ has enough room for all the subpatterns.
    //Of course, we only need to new and delete if this rows requires 
    //more space than any previous row.
    //
    size_t i;
    size_t t=1;
    for( i=0; i< d_->fields_-1; ++i )
    {
        if( d_->matches_[i].rm_so != -1 )
        {
            t += d_->matches_[i].rm_eo - d_->matches_[i].rm_so + 1 ;
        }
    }
    if( d_->datalen_ < t )
    {
        d_->datalen_=t;
        d_->data_ = new char[t];
    }

    //
    //copy each subpattern into data_, leaving null terminators, and 
    //setting colp_ pointers to the start of the appropriate portions.
    //we also have to remember the first column is reserved for the 
    // did-we-match boolean, and also that matches is therefore one less
    // long than fields.
    //
    const char *raw = d_->operand_->getRaw();
    char *dest  = d_->data_.get();
    for( i=0; i< d_->fields_-1; ++i )
    {
        if( d_->matches_[i].rm_so != -1 )
        {
            size_t len = d_->matches_[i].rm_eo - d_->matches_[i].rm_so ;
            const char *src = raw + d_->matches_[i].rm_so ;
            memcpy( dest, src, len );
            d_->colp_[i+1] = dest;
            dest = dest + len; 
            *dest++ = '\0' ;
        }
        else
        {
            d_->colp_[i+1] = RegexColumnsPriv::empty ;
        }
    }
}

const char *
RegexColumns::getField( unsigned field_index )
{
    if( field_index >= d_->fields_ )
    {
        PANICV( "field index out of bounds %u >= %u ", 
            field_index, d_->fields_ );
    }
    return d_->colp_[field_index];
}

