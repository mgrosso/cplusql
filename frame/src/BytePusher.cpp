#ident "file_id $Id: BytePusher.cpp,v 1.1 2004/07/13 22:28:39 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      BytePusher.cpp
// Author:    mgrosso 
// Created:   Sat Jul  3 10:25:56 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: BytePusher.cpp,v 1.1 2004/07/13 22:28:39 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "BytePusher.h"

class BytePusherPriv 
{
    private:
    friend class BytePusher ;

    CopyByteSourcePtr       source_; 
    pDest                   sink_ ;

    BytePusherPriv( CopyByteSourcePtr &source, pDest &sink );
};

BytePusherPriv::BytePusherPriv( CopyByteSourcePtr &source, pDest &sink )
:source_( source ), sink_(sink)
{
    ;//noop
}

BytePusher::BytePusher( CopyByteSourcePtr &source, pDest &sink )
{
    d_=new BytePusherPriv ( source, sink );
}

BytePusher::~BytePusher()
{
    delete d_;
}

void
BytePusher::run()
{
    char *bytes=NULL;
    size_t len=0;
    for( bool next = d_->source_->next( bytes, len ); 
        next; 
        next = d_->source_->next( bytes, len ))
    {
        d_->sink_->put( bytes, len );
    }
    d_->sink_->flush();
}

