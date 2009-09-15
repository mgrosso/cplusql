
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExceptionHandler.h
// Author:    mgrosso Matthew Grosso
// Created:   Fri Feb 20 13:03:47 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: ExceptionHandler.h,v 1.1 2004/02/20 21:53:03 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H 1

#include <exception>

class ExceptionHandlerPriv ;
class ExceptionHandler
{
    public:
    virtual                 ~ExceptionHandler();
    virtual     void        handle( const std::exception &e )=0;
    virtual     void        handleUnknown()=0;

    protected:
    ExceptionHandler();

    private:
    //not implemented
    ExceptionHandler( const ExceptionHandler &rhs );
    ExceptionHandler &operator=( const ExceptionHandler &rhs );
};

#endif /* EXCEPTIONHANDLER_H */
