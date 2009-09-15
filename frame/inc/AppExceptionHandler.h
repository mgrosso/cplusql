
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AppExceptionHandler.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Feb 20 13:29:51 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: AppExceptionHandler.h,v 1.2 2004/07/13 21:40:54 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef APPEXCEPTIONHANDLER_H
#define APPEXCEPTIONHANDLER_H 1

#include "ExceptionHandler.h"
#include "Config.h"

class AppExceptionHandlerPriv ;
class AppExceptionHandler : public ExceptionHandler 
{
public:
    ~AppExceptionHandler();
    static AppExceptionHandler *instance();
	static void                 cleanup();

    virtual void                configure( const Config &c );

    virtual void                handle( const std::exception &e );
    virtual void                handleUnknown();

    virtual void                handleExit( const std::exception &e );
    virtual void                handleThreadExit( const std::exception &e );
    virtual void                handleLogOnly( const std::exception &e );

    virtual void                handleExit( );
    virtual void                handleThreadExit( );
    virtual void                handleLogOnly( );

protected:
    AppExceptionHandler();

private:
    static AppExceptionHandler *instance_ ;

    friend class AppExceptionHandlerPriv ;
    AppExceptionHandlerPriv *d_;

    //not implemented
    AppExceptionHandler( const AppExceptionHandler &rhs );
    AppExceptionHandler &operator=( const AppExceptionHandler &rhs );
};

#endif /* APPEXCEPTIONHANDLER_H */
