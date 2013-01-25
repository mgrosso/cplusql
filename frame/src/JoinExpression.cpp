#ident "file_id $Id: JoinExpression.cpp,v 1.9 2004/02/12 21:16:46 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      JoinExpression.cpp
// Author:    djain Deepak Jain,,,,mgrosso,00000000
// Created:   Thu Apr 17 07:36:08 PDT 2003 on devbidb.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart Inc.  All Rights Reserved.
// 
// $Id: JoinExpression.cpp,v 1.9 2004/02/12 21:16:46 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <string.h>
#include "JoinExpression.h"
#include "Convert.h"
#include "ptr_strdup.h"
#include "BaseException.h"
#include "Parse.h"

class JoinExpressionPriv
{
    friend class JoinExpression ;
    private:
        static const int startbuf=1024;
		ExpressionList list_;
		char_ptr    psepchars_;
        const char *sepchars_ ;
		char_ptr    raw_;
        char *      buf_;
        size_t      bufsize_ ;
        char *      end_;
        char *      cursor_;
        Notifier    n_;
		inline void join();
		inline void put( const char *s );
		JoinExpressionPriv( ExpressionList & list )
        :list_(list),bufsize_(startbuf)
		{
            //we do hard code the starting size of the buffer, but it will grow dynamically
            //if needed, so its not the crime it seems at first glance.
            buf_= new char[bufsize_];
            memset( buf_, '\0', bufsize_ );
            raw_=buf_;                  //raw_ takes ownership of memory for us.
            end_=buf_+bufsize_-1;       //end_ points to last byte, which stays null.
            cursor_=buf_;
		};
};

JoinExpression::JoinExpression( ExpressionList & list, const char *sepchars)
{
	d_ = new JoinExpressionPriv(list);
	if(!d_)
	{
        THROW_NOMEM;
	}
    d_->psepchars_= ptr_strdup(sepchars);
    d_->sepchars_= d_->psepchars_.get();

	expression_list_t::iterator b = d_->list_->begin();
	expression_list_t::iterator e = d_->list_->end();
	for( ;b != e; ++b )
	{
        ListenerPtr lp( *b );
        d_->n_.registerListener( lp );
	}
}

JoinExpression::~JoinExpression()
{
	delete d_;
}

void
JoinExpression::notify( void ) 
{
    d_->n_.notify();
	d_->join();
}

const char *
JoinExpression::getRaw() 
{
	return d_->buf_;
}

const char *
JoinExpression::getRaw() const
{
	return d_->buf_;
}

void
JoinExpressionPriv::join()
{
	expression_list_t::iterator b = list_->begin();
	expression_list_t::iterator e = list_->end();
    cursor_=buf_;
    //memset( buf_, '\0', bufsize_ );
	while(b != e)
	{
		put((*b)->getRaw());
        if( ++b!=e )
        {
            put(sepchars_);
        }
	}
    *cursor_='\0';
}

void
JoinExpressionPriv::put( const char *s )
{
    char c;
    while(( c = *s++ ))
    {
        *cursor_++=c;
        if( cursor_==end_ )
        {
            char *newbuf = new char[ bufsize_ * 2 ];
            memset( newbuf, '\0', bufsize_ * 2 );
            strncpy( newbuf, buf_, bufsize_ );
            cursor_=newbuf+bufsize_;
            bufsize_ *= 2;
            end_=newbuf+bufsize_-1;
            raw_=newbuf;//will delete buf_
            buf_=newbuf;
        }
    }
}
