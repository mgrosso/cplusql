#ident "file_id $Id: Notifier.cpp,v 1.8 2005/01/24 18:41:10 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Notifier.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Sat Mar 15 17:57:44 EST 2003 on circe.looksmart.net
// Project:   
// Purpose:   
// 
// Copyright (c) 2003 LookSmart. All Rights Reserved.
// 
// $Id: Notifier.cpp,v 1.8 2005/01/24 18:41:10 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <list>
#include <algorithm>
#include "Listener.h"
#include "Notifier.h"
#include <stdio.h>
using namespace std;

#include "dbg.h"

typedef list< ptr< Listener > >  lpl_t ;
typedef list< Listener * >  rawlpl_t ;

class notifier_helper_lptr
{
    public:
    void operator()(ListenerPtr &lp)
    {
        lp->notify();
    };
};

class notifier_helper_lraw 
{
    public:
    void operator()(Listener *lp)
    {
        lp->notify();
    };
};

class NotifierPriv 
{
    private:
        friend class Notifier;
        lpl_t l_;
        rawlpl_t lr_;
        unsigned long skip_;
        unsigned long limit_;
        unsigned long count_;
        NotifierPriv( unsigned long skip, unsigned long limit )
        :skip_(skip), limit_(limit), count_(0)
        {
            ;//noop
        };
        bool    finished();
};

Notifier::Notifier()
{
    d_=new NotifierPriv(0,0);
    //fprintf(stderr, "Notifier() %x\n", (unsigned )this);
}

Notifier::Notifier(unsigned long skip, unsigned long limit )
{
    d_=new NotifierPriv(skip, limit );
    //fprintf(stderr, "Notifier() %x\n", (unsigned )this);
}

Notifier::~Notifier()
{
    //fprintf(stderr, "~Notifier top %x\n", (unsigned )this);
    delete d_;
    //fprintf(stderr, "~Notifier bottom %x\n", (unsigned )this);
}

void
Notifier::registerListener( ListenerPtr &l )
{
    //FDBG_SII( "Notifier::registerListener() iterator", this, l.get());
    d_->l_.push_back( l );
}

void
Notifier::removeListener( ListenerPtr &l )
{
    d_->l_.remove( l );
}

void
Notifier::registerListener( Listener *l )
{
    //FDBG_SII( "Notifier::registerListener() pointer", this, l);
    d_->lr_.push_back( l );
}

void
Notifier::removeListener( Listener *l )
{
    d_->lr_.remove( l );
}

bool
Notifier::finished()
{
    return d_->finished();
}

bool
NotifierPriv::finished()
{
    return limit_ && count_ - skip_ > limit_ ;
}

void
Notifier::notify()
{
    if( ++ d_->count_ <= d_->skip_ )
    {
        return;
    }
    if( d_->finished())
    {
        return;
    }
    //notifier_helper_lptr nhp;
    //for_each( d_->l_.begin(), d_->l_.end(), nhp);
    //notifier_helper_lraw nhr;
    //for_each( d_->lr_.begin(), d_->lr_.end(), nhr);

    //FDBG_SII( "Notifier::notify() this, lsize", this, d_->l_.size() );
    //FDBG_SII( "Notifier::notify() this, lrsize", this, d_->lr_.size() );
    lpl_t::iterator b;
    lpl_t::iterator e;
    for( b=d_->l_.begin(), e=d_->l_.end(); b!=e; ++b )
    {
        //FDBG_SII( "Notifier::notify() iterator a", this, (*b).get());
        (*b)->notify();
        //FDBG_SII( "Notifier::notify() iterator b", this, (*b).get());
    }
    rawlpl_t::iterator bi;
    rawlpl_t::iterator ei;
    for( bi=d_->lr_.begin(), ei=d_->lr_.end(); bi!=ei; ++bi )
    {
        //FDBG_SII( "Notifier::notify() pointer a", this, (*bi));
        (*bi)->notify();
        //FDBG_SII( "Notifier::notify() pointer b", this, (*bi));
    }
}

void
Notifier::notifyLast()
{
    //notifier_helper_lptr nhp;
    //for_each( d_->l_.begin(), d_->l_.end(), nhp);
    //notifier_helper_lraw nhr;
    //for_each( d_->lr_.begin(), d_->lr_.end(), nhr);

    //FDBG_SII( "Notifier::notifyLast() this, lsize", this, d_->l_.size() );
    //FDBG_SII( "Notifier::notifyLast() this, lrsize", this, d_->lr_.size() );
    lpl_t::iterator b;
    lpl_t::iterator e;
    for( b=d_->l_.begin(), e=d_->l_.end(); b!=e; ++b )
    {
        //FDBG_SII( "Notifier::notifyLast() iterator a", this, (*b).get());
        (*b)->notifyLast();
        //FDBG_SII( "Notifier::notifyLast() iterator b", this, (*b).get());
    }
    rawlpl_t::iterator bi;
    rawlpl_t::iterator ei;
    for( bi=d_->lr_.begin(), ei=d_->lr_.end(); bi!=ei; ++bi )
    {
        //FDBG_SII( "Notifier::notifyLast() pointer a", this, (*bi));
        (*bi)->notifyLast();
        //FDBG_SII( "Notifier::notifyLast() pointer b", this, (*bi));
    }
}

