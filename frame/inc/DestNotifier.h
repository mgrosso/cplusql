
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestNotifier.h
// Author:    mgrosso 
// Created:   Fri Jul  2 21:56:23 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: DestNotifier.h,v 1.2 2004/08/11 21:42:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DESTNOTIFIER_H
#define DESTNOTIFIER_H 1

#include "Dest.h"

class DestNotifierPriv ;
class DestNotifier : public Dest
{
    public:
    virtual void            registerListener( pDest &l );
    virtual void            removeListener( pDest &l );
    virtual void            registerListener( Dest *l );
    virtual void            removeListener( Dest *l );
    virtual void            put( const char *bytes, int len );
    virtual void            put( const char *bytes );
    virtual void            flush();

    virtual ~DestNotifier();
    DestNotifier();

    protected:
    private:
    DestNotifierPriv *d_;
    //not implemented
    DestNotifier( const DestNotifier &rhs );
    DestNotifier &operator=( const DestNotifier &rhs );
};

#endif /* DESTNOTIFIER_H */
