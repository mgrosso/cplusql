#ident "file_id $Id: AbstractRelation.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      AbstractRelation.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Fri Sep  6 00:26:32 EDT 2002 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 2002 LookSmart Inc.  All Rights Reserved.
// 
// $Id: AbstractRelation.cpp,v 1.6 2004/01/16 21:45:26 djain Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#include "AbstractRelation.h"
#include "Notifier.h"
#include "dbg.h"

AbstractRelation::AbstractRelation()
{
    n_= new Notifier();
}

AbstractRelation::~AbstractRelation()
{
    //fprintf(stderr, "~AbstractRelation top %x\n", (unsigned )this);
    delete n_ ;
    //fprintf(stderr, "~AbstractRelation bottom %x\n", (unsigned )this);
}

void
AbstractRelation::notify()
{
    //fprintf(stderr, "AbstractRelation::next(): this %x n %x name %s line %u\n", 
    //    reinterpret_cast<unsigned int>(this), 
    //    reinterpret_cast<unsigned int>(n_), 
    //    getRelationName(), lineNum());
    //
    //FDBG_SI( "AbstractRelation::notify() top", this );
    if( childNotify())
    {
        //FDBG_SII( "AbstractRelation::notify() yes", this, n_ );
        n_->notify();
    }
    //FDBG_SI( "AbstractRelation::notify() bottom", this );
}

void
AbstractRelation::notifyLast()
{
    //FDBG_SI( "AbstractRelation::notifyLast() top", this );
    if( childNotifyLast())
    {
        //FDBG_SII( "AbstractRelation::notifyLast() yes", this, n_ );
        n_->notifyLast();
    }
    //FDBG_SI( "AbstractRelation::notifyLast() bottom", this );
}

bool
AbstractRelation::childNotifyLast()
{
    return true;
}

void
AbstractRelation::registerListener( ListenerPtr &l )
{
    //FDBG_SII( "AbstractRelation::registerListener() ptr", this, l.get() );
    n_->registerListener( l );
}

void
AbstractRelation::registerListener( Listener *l )
{
    //FDBG_SII( "AbstractRelation::registerListener() pointer", this, l );
    n_->registerListener( l );
}

void
AbstractRelation::removeListener( ListenerPtr &l )
{
    //FDBG_SII( "AbstractRelation::removeListener() ptr", this, l.get() );
    n_->removeListener( l );
}

void
AbstractRelation::removeListener( Listener *l )
{
    //FDBG_SII( "AbstractRelation::removeListener() pointer", this, l );
    n_->removeListener( l );
}

