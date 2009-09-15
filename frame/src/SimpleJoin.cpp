#ident "file_id $Id: SimpleJoin.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SimpleJoin.cpp
// Author:    mgrosso 
// Created:   Sun Jul  7 21:47:02 PDT 2002 on sf-devdw001.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart. All Rights Reserved.
// 
// $Id: SimpleJoin.cpp,v 1.4 2004/01/16 21:45:27 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "SimpleJoin.h"
#include "SimpleJoinPriv.h"
#include "BaseException.h"
#include "AbstractDelimitedSource.h"

SimpleJoin::SimpleJoin()
{
    d_= new SimpleJoinPriv();
    d_->j_= new kcpmap_t();
}

SimpleJoin::~SimpleJoin()
{
    delete d_ ;
}

void
SimpleJoin::loadMap( int keycol, DelimitedSourcePtr right )
{
    while( right->next() )
    {
        out_key_t k = right->getKey( keycol );
        char_ptr cp = right->getCharPtr();
        d_->j_->insert( pair< out_key_t, char_ptr >(k,cp));
    }
}

void
SimpleJoin::processJoin( int keycol, DelimitedSourcePtr left,
    pDest pd, const char *output_delimiter )
{
    kcpmap_t::iterator e = d_->j_->end();
    kcpmap_t::iterator f ;
    while( left->next())
    {
        out_key_t k = left->getKey( keycol );
        if( e!=( f = d_->j_->find( k )))
        {
            do{
                char_ptr ltp = left->getCharPtr();
                char_ptr rtp = (*f).second;

                pd->put( ltp.get());
                pd->put( output_delimiter );
                pd->put( rtp.get() );
                pd->put( "\n" );

            }while( (*(++f)).first == k );
        }
    }
}


