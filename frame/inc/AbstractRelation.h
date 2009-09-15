// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Relation.h
// Author:    mgrosso Matthew Grosso
// Created:   Tue Dec  5 17:30:23 EST 2000 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: AbstractRelation.h,v 1.6 2004/09/20 20:57:47 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef ABSTRACT_RELATION_H
#define ABSTRACT_RELATION_H 1

#include "AbstractDelimitedSource.h"
#include "AbstractNotifier.h"
#include "AbstractRelationMeta.h"

class Notifier ;

class AbstractRelation : 
    virtual public AbstractNotifier, 
    virtual public AbstractDelimitedSource, 
    virtual public AbstractRelationMeta
{
    protected:
    AbstractRelation();

    //This is where children can make filtering choices or aggregations without
    //re-implementing the Notifier functionality. children should return true
    //only after having notified any where clauses, usercolumns, etc...
    //childNotifyLast() works the same way, except that there is a default
    //implementation which returns false.
    //
    virtual bool            childNotify()=0; 
    virtual bool            childNotifyLast();

    public:
    virtual                 ~AbstractRelation();

    //default implementation of notify() and notifyLast at this level calls
    //childNotify(), and, if the result is true, it notifies listeners.
    //
    virtual void            notify(); 
    virtual void            notifyLast();

    virtual char_ptr        getCharPtr()=0 ;

    //implemented methods
    virtual void            registerListener( ListenerPtr &lp );
    virtual void            registerListener( Listener *lp );
    virtual void            removeListener( ListenerPtr &lp );
    virtual void            removeListener( Listener *lp );

    protected:
    Notifier    *n_;

    private:

    //not implemented
    AbstractRelation( const AbstractRelation &rhs);
    AbstractRelation &operator=( const AbstractRelation &rhs);
};

typedef ptr< AbstractRelation > RelationPtr ;

#endif /* ABSTRACT_RELATION_H */
