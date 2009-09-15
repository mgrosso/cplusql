
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      StringArrayListener.h
// Author:    mgrosso 
// Created:   Thu Jul  8 00:19:49 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: StringArrayListener.h,v 1.1 2004/07/13 22:44:03 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef STRINGARRAYLISTENER_H
#define STRINGARRAYLISTENER_H 1

#include "Listener.h"
#include "StringArray.h"
#include "AbstractRelation.h"

class StringArrayListenerPriv ;
class StringArrayListener : public Listener
{
    public:
    StringArrayListener( StringArrayPtr &sa, RelationPtr &r );
    virtual ~StringArrayListener();

    virtual void notify();

    private:
    StringArrayListenerPriv *d_;
    //not implemented
    StringArrayListener();
    StringArrayListener( const StringArrayListener &rhs );
    StringArrayListener &operator=( const StringArrayListener &rhs );
};



#endif /* STRINGARRAYLISTENER_H */
