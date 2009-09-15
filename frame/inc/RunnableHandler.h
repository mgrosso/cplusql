
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      RunnableHandler.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Feb 20 12:38:57 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: RunnableHandler.h,v 1.2 2004/02/28 03:10:38 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef RUNNABLEHANDLER_H
#define RUNNABLEHANDLER_H 1

#include "Event.h"

class RunnableHandlerPriv ;
class RunnableHandler : public EventHandler
{
    public:
                        RunnableHandler( bool delete_runnable );
                        RunnableHandler();//same as delete_runnable=true
    virtual             ~RunnableHandler();
    virtual void        handleEvent( void *e );

    private:
    RunnableHandlerPriv *d_;
    //not implemented
    RunnableHandler( const RunnableHandler &rhs );
    RunnableHandler &operator=( const RunnableHandler &rhs );
};



#endif /* RUNNABLEHANDLER_H */
