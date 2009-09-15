#ident "file_id $Id: DestNotifier.cpp,v 1.2 2004/08/11 21:42:43 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      DestNotifier.cpp
// Author:    mgrosso 
// Created:   Fri Jul  2 21:56:23 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: DestNotifier.cpp,v 1.2 2004/08/11 21:42:43 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <list>
#include <algorithm>
#include "Dest.h"
#include "DestNotifier.h"
#include "dbg.h"
#include <stdio.h>
using namespace std;


typedef list< pDest >  blpl_t ;
typedef list< Dest * >  brawlpl_t ;

class DestNotifierPriv 
{
    private:
        friend class DestNotifier;
        blpl_t l_;
        brawlpl_t lr_;
};

DestNotifier::DestNotifier()
{
    d_=new DestNotifierPriv();
}

DestNotifier::~DestNotifier()
{
    //fprintf(stderr, "~DestNotifier top %x\n", (unsigned )this);
    delete d_;
    //fprintf(stderr, "~DestNotifier bottom %x\n", (unsigned )this);
}

void
DestNotifier::registerListener( pDest &l )
{
    //FDBG_SII( "DestNotifier::registerListener() iterator", this, l.get());
    d_->l_.push_back( l );
}

void
DestNotifier::removeListener( pDest &l )
{
    d_->l_.remove( l );
}

void
DestNotifier::registerListener( Dest *l )
{
    //FDBG_SII( "DestNotifier::registerListener() pointer", this, l);
    d_->lr_.push_back( l );
}

void
DestNotifier::removeListener( Dest *l )
{
    d_->lr_.remove( l );
}

void
DestNotifier::put( const char *bytes )
{
    put( bytes, strlen( bytes ));
}

void
DestNotifier::put( const char *bytes, int len )
{
    //notifier_helper_lptr nhp;
    //for_each( d_->l_.begin(), d_->l_.end(), nhp);
    //notifier_helper_lraw nhr;
    //for_each( d_->lr_.begin(), d_->lr_.end(), nhr);

    //FDBG_SII( "DestNotifier::notify() this, lsize", this, d_->l_.size() );
    //FDBG_SII( "DestNotifier::notify() this, lrsize", this, d_->lr_.size() );
    blpl_t::iterator b;
    blpl_t::iterator e;
    for( b=d_->l_.begin(), e=d_->l_.end(); b!=e; ++b )
    {
        //FDBG_SII( "DestNotifier::notify() iterator a", this, (*b).get());
        (*b)->put( bytes, len );
        //FDBG_SII( "DestNotifier::notify() iterator b", this, (*b).get());
    }
    brawlpl_t::iterator bi;
    brawlpl_t::iterator ei;
    for( bi=d_->lr_.begin(), ei=d_->lr_.end(); bi!=ei; ++bi )
    {
        //FDBG_SII( "DestNotifier::notify() pointer a", this, (*bi));
        (*bi)->put( bytes, len );
        //FDBG_SII( "DestNotifier::notify() pointer b", this, (*bi));
    }
}

void
DestNotifier::flush()
{
    //notifier_helper_lptr nhp;
    //for_each( d_->l_.begin(), d_->l_.end(), nhp);
    //notifier_helper_lraw nhr;
    //for_each( d_->lr_.begin(), d_->lr_.end(), nhr);

    //FDBG_SII( "DestNotifier::notifyLast() this, lsize", this, d_->l_.size() );
    //FDBG_SII( "DestNotifier::notifyLast() this, lrsize", this, d_->lr_.size() );
    blpl_t::iterator b;
    blpl_t::iterator e;
    for( b=d_->l_.begin(), e=d_->l_.end(); b!=e; ++b )
    {
        //FDBG_SII( "DestNotifier::notifyLast() iterator a", this, (*b).get());
        (*b)->flush();
        //FDBG_SII( "DestNotifier::notifyLast() iterator b", this, (*b).get());
    }
    brawlpl_t::iterator bi;
    brawlpl_t::iterator ei;
    for( bi=d_->lr_.begin(), ei=d_->lr_.end(); bi!=ei; ++bi )
    {
        //FDBG_SII( "DestNotifier::notifyLast() pointer a", this, (*bi));
        (*bi)->flush();
        //FDBG_SII( "DestNotifier::notifyLast() pointer b", this, (*bi));
    }
}


