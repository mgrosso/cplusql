#ident "file_id $Id: DelimitedColumn.cpp,v 1.7 2004/06/02 21:35:10 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DelimitedColumn.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Nov  9 18:37:22 EST 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: DelimitedColumn.cpp,v 1.7 2004/06/02 21:35:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "DelimitedColumn.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"
#include "Parse.h"
#include <stdio.h>

class DelimitedColumnPriv
{
    friend class DelimitedColumn;
    private:
    ~DelimitedColumnPriv();
    DelimitedColumnPriv( 
        const ExpressionPtr & operand, 
        const char *sepchars,
        size_t num_fields, 
        bool expect_min,
        bool no_extra );
    ExpressionPtr        operand_;
    const char **        colp_ ;
    char_ptr             data_ ;
    size_t               datalen_ ;
    size_t               num_fields_;
    size_t               parsed_fields_;
    bool                 expect_min_;
    bool                 no_extra_;

    enum charAction { enull=0, ecopy, edelim };
    charAction          actions_[ UCHAR_MAX + 1 ];
};

DelimitedColumnPriv::~DelimitedColumnPriv( )
{
    delete colp_;
}

DelimitedColumnPriv::DelimitedColumnPriv( 
    const ExpressionPtr & operand, 
    const char *sepchars,
    size_t num_fields, 
    bool expect_min,
    bool no_extra )
:operand_(operand),
    data_(NULL), datalen_(0), 
    num_fields_(num_fields), parsed_fields_(0), 
    expect_min_(expect_min), no_extra_(no_extra)
{
    colp_ = new const char *[num_fields_];
    memset( colp_, '\0', num_fields_ * sizeof( const char *));
    for( unsigned char i=0; i < UCHAR_MAX ; ++i )
    {
        actions_[ i ] = ecopy;
    }
    actions_[ 255 ]=ecopy ;
    for( const char *i=sepchars; *i ; ++i )
    {
        actions_[ static_cast<unsigned char >(*i) ] = edelim;
    }
    actions_[ static_cast< unsigned char>('\0')]=enull ;
}

DelimitedColumn::DelimitedColumn( const ExpressionPtr & col, const char *sepchars, size_t num_fields, bool expect_min, bool no_extra )
{
    d_=new DelimitedColumnPriv( 
        col, sepchars, num_fields, expect_min, no_extra);
}

const char *
DelimitedColumn::getField(unsigned field_index)
{
    if ( d_->num_fields_ <= field_index)
    {
        PANICV("DelimitedColumn::getElement() %u out of bounds %u", 
            field_index, d_->num_fields_ );
    }
    if ( d_->parsed_fields_ <= field_index)
    {
        if(!d_->expect_min_)
        {
            return "";
        }
        else
        {
            PANICV("DelimitedColumn::getElement() %u out of bounds %u", 
                field_index, d_->parsed_fields_ );
        }
    }
    return d_->colp_[field_index];
}

void
DelimitedColumn::parse(const char * value)
{
    if( !value )
    {
        PANICV( "null operand for delimited column");
    }
    size_t required = 1 + strlen( value );
    if( d_->datalen_ < required )
    {
        d_->data_ = new char[ required ];
    }
    memset( d_->data_.get(), '\0', d_->datalen_ );
    const char *v = value;
    char *d = d_->data_.get();
    //char *e = d_->data_.get() + d_->datalen_ -1 ;
    d_->parsed_fields_ = 1;
    d_->colp_[0] = d ;
    while( *v )
    {
        switch( d_->actions_[ static_cast<unsigned char >(*v) ] )
        {
            case DelimitedColumnPriv::ecopy :
                *d++=*v++;
                break;
            case DelimitedColumnPriv::edelim :
                if( d_->parsed_fields_ < d_->num_fields_ )
                {
                    *d++='\0';
                    ++v;
                    d_->colp_[ d_->parsed_fields_++ ] = d ;
                }
                else
                {
                    *d++=*v++ ;
                }
                break;
            case DelimitedColumnPriv::enull :
                PANICV("should never get here because we're inside *v");
                break;
            default:
                PANICV("should never get here");
                break;
        }
    }
    *d='\0';
    if( d_->expect_min_ && d_->parsed_fields_ < d_->num_fields_ )
    {
        PANICV("Not enough delimiters, %u < %u, char %u in %s", 
            d_->parsed_fields_, d_->num_fields_, v - value, value );
    }
    for( size_t c = d_->parsed_fields_ ; c < d_->num_fields_  ; ++c)
    {
        d_->colp_[c] = ""; //not enough delimiters, but not strict
    }
    if( d_->no_extra_ )
    {
        v = d_->colp_[ d_->parsed_fields_ -1 ];
        while( *v && d_->actions_[ static_cast<unsigned char >(*v) ] 
            != DelimitedColumnPriv::edelim )
        {
            ++v;
        }
        if( *v )
        {
            PANICV("Too many delimiters, char %u in %s", 
                v - d_->data_.get(), value );
        }
    }
}

DelimitedColumn::~DelimitedColumn()
{
    delete d_;
}

void
DelimitedColumn::notify()
{
    d_->operand_->notify();
    parse(d_->operand_->getRaw());
}

