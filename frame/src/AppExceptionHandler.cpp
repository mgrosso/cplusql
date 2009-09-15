#ident "file_id $Id: AppExceptionHandler.cpp,v 1.2 2004/07/13 21:40:55 sjackson Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AppExceptionHandler.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Fri Feb 20 13:29:51 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: AppExceptionHandler.cpp,v 1.2 2004/07/13 21:40:55 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "AppExceptionHandler.h"
#include "AppLog.h"
#include "Thread.h"
#include <stdlib.h>

class AppExceptionHandlerPriv 
{
    private:
    friend class AppExceptionHandler ;
    static const char *     unknown_ ;
    bool                    defexit_ ;
    Config                  conf_ ;

    AppExceptionHandlerPriv();
    void log( const char * func , const char *what=unknown_  );
};

AppExceptionHandler *
AppExceptionHandler::instance_ = new AppExceptionHandler();

AppExceptionHandlerPriv::AppExceptionHandlerPriv()
:defexit_(true)
{
    //noop
};

void
AppExceptionHandlerPriv::log( const char * func , const char *what )
{
    APPLOG_ABORT( "AppExceptionHandler::%s(): %s", func, what );
};

const char *
AppExceptionHandlerPriv::unknown_  = "unknown exception" ;


AppExceptionHandler *
AppExceptionHandler::instance()
{
    return instance_ ;
}

void
AppExceptionHandler::cleanup()
{
	if (instance_ != (AppExceptionHandler *) 0)
	{
		delete instance_;
		instance_ = (AppExceptionHandler *) 0;
	}
}

AppExceptionHandler::AppExceptionHandler()
{
    d_=new AppExceptionHandlerPriv ();
}

AppExceptionHandler::~AppExceptionHandler()
{
    delete d_;
}

void
AppExceptionHandler::handle( const std::exception &e )
{
    handleLogOnly( e );
    if(d_->defexit_)
    {
        exit(1);
    }
}

void
AppExceptionHandler::handleUnknown()
{
    if(d_->defexit_)
    {
        handleExit();
    }else
    {
        handleLogOnly();
    }
}

void
AppExceptionHandler::handleExit( const std::exception &e )
{
    d_->log( "handleExit", e.what());
    exit(1);
}

void
AppExceptionHandler::handleThreadExit( const std::exception &e )
{
    d_->log( "handleThreadExit", e.what());
    Thread::exit();
}

void
AppExceptionHandler::handleLogOnly( const std::exception &e )
{
    d_->log( "handleLogOnly", e.what());
}

void
AppExceptionHandler::handleExit( )
{
    d_->log( "handleExit" );
    exit(1);
}

void
AppExceptionHandler::handleThreadExit( )
{
    d_->log( "handleThreadExit" );
    Thread::exit();
}

void
AppExceptionHandler::handleLogOnly( )
{
    d_->log( "handleLogOnly" );
}

void
AppExceptionHandler::configure( const Config &c )
{
    d_->conf_ =c;
    d_->defexit_ = d_->conf_.getBool( "APPEXCEPTION_EXIT", "1" );
}

