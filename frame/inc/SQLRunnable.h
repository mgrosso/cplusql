
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SQLRunnable.h
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb 16 18:43:08 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: SQLRunnable.h,v 1.1 2004/02/20 21:53:03 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef SQLRUNNABLE_H
#define SQLRUNNABLE_H 1
#include "Runnable.h"
#include "db/DBConnection.h"

class SQLRunnablePriv ;
class SQLRunnable : public Runnable
{
    public:
                    SQLRunnable( 
                        DBConnectionPtr &conn, 
                        const char *sql, 
                        int rows_affected=-1 
                        );
    virtual         ~SQLRunnable();
    virtual void    run();

    private:
    SQLRunnablePriv *d_;
    //not implemented
    SQLRunnable();
    SQLRunnable( const SQLRunnable &rhs );
    SQLRunnable &operator=( const SQLRunnable &rhs );
};

#endif /* SQLRUNNABLE_H */
