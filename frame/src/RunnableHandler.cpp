#ident "file_id $Id: RunnableHandler.cpp,v 1.2 2004/02/28 03:10:38 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RunnableHandler.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Fri Feb 20 12:38:57 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: RunnableHandler.cpp,v 1.2 2004/02/28 03:10:38 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "RunnableHandler.h"
#include "Runnable.h"

class RunnableHandlerPriv 
{
    private:
    friend class RunnableHandler ;
    bool del_;
    RunnableHandlerPriv( bool d )
    :del_(d)
    {
        ;//noop
    };
};

RunnableHandler::RunnableHandler( bool delete_runnable )
{
    d_=new RunnableHandlerPriv ( delete_runnable );
}

RunnableHandler::RunnableHandler( )
{
    d_=new RunnableHandlerPriv ( true );
}

RunnableHandler::~RunnableHandler()
{
    delete d_;
}

void
RunnableHandler::handleEvent( void * e)
{
    Runnable *r = reinterpret_cast< Runnable * > (e);
    r->run();
    if( d_->del_ )
    {
        delete r;
    }
}

