#ident "file_id $Id: StringArrayListener.cpp,v 1.2 2004/08/11 21:42:44 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArrayListener.cpp
// Author:    mgrosso 
// Created:   Thu Jul  8 00:19:49 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArrayListener.cpp,v 1.2 2004/08/11 21:42:44 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "StringArrayListener.h"
#include "AppLog.h"

class StringArrayListenerPriv 
{
    private:
    friend class StringArrayListener ;
    StringArrayPtr                          sa_ ;
    RelationPtr                             r_ ;
    size_t                                  cols_ ;

    StringArrayListenerPriv(StringArrayPtr &sa, RelationPtr &r );
};

StringArrayListenerPriv::StringArrayListenerPriv(StringArrayPtr &sa, RelationPtr &r )
:sa_(sa), r_(r), cols_( r_->getNumColumns())
{
    ;//noop
}

StringArrayListener::StringArrayListener( StringArrayPtr &sa, RelationPtr &r )
{
    d_=new StringArrayListenerPriv ( sa, r );
    r->registerListener( this );
}

StringArrayListener::~StringArrayListener()
{
    delete d_;
}

void
StringArrayListener::notify()
{
    //APPLOG_DBG("top");
    for( size_t i=0; i< d_->cols_ ; ++i )
    {
        //APPLOG_DBG(".");
        d_->sa_->copy( d_->r_->getRaw( i ));
    }
    //APPLOG_DBG("bottom");
}


