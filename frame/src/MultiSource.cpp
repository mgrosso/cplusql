#ident "file_id $Id: MultiSource.cpp,v 1.3 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      MultiSource.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Sep 28 00:26:11 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: MultiSource.cpp,v 1.3 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include <list>
#include "MultiSource.h"
#include "BaseException.h"
#include "dbg.h"

#define MULTIDBG( s ) FDBG( MultiSource, s, 0 )

class MultiSourcePriv
{
    friend class MultiSource ;
    private:
    MultiSourcePriv(){ line_=0; done_=0;};
    int line_;
    int done_;
    list< SourcePtr > src_ ;
};

MultiSource::MultiSource()
{
    d_=new MultiSourcePriv() ;
    if( NULL== d_ )
    {
        THROW_NOMEM ;
    }
    d_->line_=0;
    d_->done_=0;
}

MultiSource::~MultiSource()
{
    delete d_;
}

void
MultiSource::add( SourcePtr sp )
{
    d_->src_.push_back( sp );
};

bool
MultiSource::next()
{
    if( d_->done_ )
    {
        MULTIDBG( "next(), done allready, return false" );
        return false ;
    }
    while( ! d_->src_.front()->next() )
    {
        MULTIDBG( "next(), top while" );
        d_->src_.pop_front();
        if( d_->src_.empty())
        {
            MULTIDBG( "next(), list is empty, return false" );
            d_->done_=1;
            return false;
        }
    }
    MULTIDBG( "next(), bottom, returning true" );
    ++ d_->line_;
    return true ;
}

char_ptr    
MultiSource::getCharPtr()
{
    if( d_->done_ )
    {
        MULTIDBG( "getCharPtr(), done allready, hanging" );
        assert(0);
    }
    return d_->src_.front()->getCharPtr() ;
}

int         
MultiSource::lineNum()
{
    return d_->line_ ;
}

